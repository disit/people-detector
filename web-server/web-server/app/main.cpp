/**
 * Copyright (C) 2021 Axis Communications AB, Lund, Sweden
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <libmonkey.h>
#include <unistd.h>
#include <stdlib.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <glib.h>
#include <cstring>
#include <thread>
#include <vector>
#include <torch/script.h>
#include <torch/torch.h>
#include <fstream>
#include <chrono>
#include <list>
#include <ctime>


#include "image.h"
#include "imgprovider.h"
#include "mqtt.hpp"
//#include "mjpeg_streamer.hpp"
#include "base64.h"

using namespace cv;
using namespace std;
//using MJPEGStreamer = nadjieb::MJPEGStreamer;

std::thread t1;
bool finishedThread = true;
std::string currentImage;
std::string currentBox;


int list_2(const mklib_session *sr, const char *vhost, const char *url,
                  const char *get, unsigned long getlen,
                  const char *post, unsigned long postlen,
                  unsigned int *status, const char **content, unsigned long *content_len,
                  char *header);

cv::Mat bgr_mat;
//MJPEGStreamer streamer;

/* The callback setting interface can't check the callback for compatibility.
 * This makes sure the callback function has the right arguments. */
cb_data listf = list_2;
torch::jit::script::Module module;
ImgProvider_t* provider = NULL;
bool successInitialize = false;
unsigned int width = 1024;
unsigned int height = 576;

void saveSettings(){
    ofstream outFile;
    outFile.open("/usr/local/packages/disit_app/html/config.txt",std::ofstream::out);
    outFile << address << std::endl;
    outFile << port << std::endl;
    outFile << topic_name << std::endl;
    outFile << client_id << std::endl;
    outFile << qos << std::endl;
    outFile << mqtt_timeout << std::endl;
    outFile << times_execution_onstartup << std::endl;
    outFile << sleepTime << std::endl;
    outFile << mqtt_on_value << std::endl;
    outFile.close();
}

void readConfigFile(){
    std::ifstream inFile;
    inFile.open("/usr/local/packages/disit_app/html/config.txt");
    inFile >> address >> port >> topic_name >> client_id >> qos >> mqtt_timeout
           >> times_execution_onstartup >> sleepTime >> mqtt_on_value;
    inFile.close();

    if(address.empty() && port.empty() &&
    topic_name.empty() && client_id.empty() &&
    qos.empty() && mqtt_timeout.empty() &&
    times_execution_onstartup.empty() && sleepTime.empty() && mqtt_on_value.empty()){
        address = "tcp://localhost";
        port = "1883";
        topic_name = "disit/object_detection";
        client_id = "testClientID";
        qos = "0";
        mqtt_timeout = "10000";
        times_execution_onstartup = "1";
        sleepTime = "5000";
        mqtt_on_value = "on";
        saveSettings();

    }
}

torch::jit::script::Module initialize_jit_model(){

    torch::jit::script::Module local_module;

    try {
        // Deserialize the ScriptModule from a file using torch::jit::load().
        syslog(LOG_INFO,"loading model in /var/spool/storage/SD_DISK/lib/traced_51_model.pt");
        local_module = torch::jit::load("/var/spool/storage/SD_DISK/lib/traced_51_model.pt");
    }
    catch (const c10::Error& e) {
        syslog(LOG_INFO, "error loading the model");
        if(successInitialize){
            return module;
        }else {
            return local_module;
        }
    }

    return local_module;
}

std::vector<torch::Tensor> non_max_suppression(torch::Tensor preds, float score_thresh = 0.01, float iou_thresh = 0.35)
{
    std::vector<torch::Tensor> output;
    for (size_t i=0; i < preds.sizes()[0]; ++i)
    {
        torch::Tensor pred = preds.select(0, i);

        // Filter by scores
        torch::Tensor scores = pred.select(1, 4) * std::get<0>( torch::max(pred.slice(1, 5, pred.sizes()[1]), 1));
        pred = torch::index_select(pred, 0, torch::nonzero(scores > score_thresh).select(1, 0));
        if (pred.sizes()[0] == 0) continue;

        // (center_x, center_y, w, h) to (left, top, right, bottom)
        pred.select(1, 0) = pred.select(1, 0) - pred.select(1, 2) / 2;
        pred.select(1, 1) = pred.select(1, 1) - pred.select(1, 3) / 2;
        pred.select(1, 2) = pred.select(1, 0) + pred.select(1, 2);
        pred.select(1, 3) = pred.select(1, 1) + pred.select(1, 3);

        // Computing scores and classes
        std::tuple<torch::Tensor, torch::Tensor> max_tuple = torch::max(pred.slice(1, 5, pred.sizes()[1]), 1);
        pred.select(1, 4) = pred.select(1, 4) * std::get<0>(max_tuple);
        pred.select(1, 5) = std::get<1>(max_tuple);

        torch::Tensor  dets = pred.slice(1, 0, 6);

        torch::Tensor keep = torch::empty({dets.sizes()[0]});
        torch::Tensor areas = (dets.select(1, 3) - dets.select(1, 1)) * (dets.select(1, 2) - dets.select(1, 0));
        std::tuple<torch::Tensor, torch::Tensor> indexes_tuple = torch::sort(dets.select(1, 4), 0, 1);
        torch::Tensor v = std::get<0>(indexes_tuple);
        torch::Tensor indexes = std::get<1>(indexes_tuple);
        int count = 0;
        while (indexes.sizes()[0] > 0)
        {
            keep[count] = (indexes[0].item().toInt());
            count += 1;

            // Computing overlaps
            torch::Tensor lefts = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor tops = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor rights = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor bottoms = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor widths = torch::empty(indexes.sizes()[0] - 1);
            torch::Tensor heights = torch::empty(indexes.sizes()[0] - 1);
            for (size_t i=0; i<indexes.sizes()[0] - 1; ++i)
            {
                lefts[i] = std::max(dets[indexes[0]][0].item().toFloat(), dets[indexes[i + 1]][0].item().toFloat());
                tops[i] = std::max(dets[indexes[0]][1].item().toFloat(), dets[indexes[i + 1]][1].item().toFloat());
                rights[i] = std::min(dets[indexes[0]][2].item().toFloat(), dets[indexes[i + 1]][2].item().toFloat());
                bottoms[i] = std::min(dets[indexes[0]][3].item().toFloat(), dets[indexes[i + 1]][3].item().toFloat());
                widths[i] = std::max(float(0), rights[i].item().toFloat() - lefts[i].item().toFloat());
                heights[i] = std::max(float(0), bottoms[i].item().toFloat() - tops[i].item().toFloat());
            }
            torch::Tensor overlaps = widths * heights;

            // FIlter by IOUs
            torch::Tensor ious = overlaps / (areas.select(0, indexes[0].item().toInt()) + torch::index_select(areas, 0, indexes.slice(0, 1, indexes.sizes()[0])) - overlaps);
            indexes = torch::index_select(indexes, 0, torch::nonzero(ious <= iou_thresh).select(1, 0) + 1);
        }
        keep = keep.toType(torch::kInt64);
        output.push_back(torch::index_select(dets, 0, keep.slice(0, 0, count)));
    }
    return output;
}


void motion_detector(signed int times){

    finishedThread = false;

    syslog(LOG_INFO, "Thread entered");
    syslog(LOG_INFO, "Running OpenCV with VDO as video source");
    syslog(LOG_INFO, "Running motion detector for %d times", times);

    int resize_width = 768; //640;
    int resize_height = 576; //384;

    std::vector <std::string> classnames;
    classnames.push_back("person");
    classnames.push_back("carrozzina");
    classnames.push_back("bike");

    torch::DeviceType device_type = at::kCPU;
    //if (torch::cuda::is_available())
    //    device_type = at::kCUDA;

    std::vector <cv::Scalar> colors{cv::Scalar(173, 255, 47), cv::Scalar(64, 67, 147),
                                    cv::Scalar(210, 180, 80), cv::Scalar(120, 30, 130), cv::Scalar(255, 128, 128)};

    if(successInitialize == false) {
        syslog(LOG_INFO, "Initializing model...");
        module = initialize_jit_model();

        // The desired width and height of the BGR frame

        // chooseStreamResolution gets the least resource intensive stream
        // that exceeds or equals the desired resolution specified above
        unsigned int streamWidth = 0;
        unsigned int streamHeight = 0;
        if (!chooseStreamResolution(width, height, &streamWidth,
                                    &streamHeight) || streamHeight == 0) {
            width = 768;
            height = 576;
            streamWidth = 0;
            streamHeight = 0;
            if (!chooseStreamResolution(width, height, &streamWidth,
                                        &streamHeight)) {
                syslog(LOG_ERR, "%s: Failed choosing stream resolution", __func__);
                exit(1);
            }
        }

        syslog(LOG_INFO, "Creating VDO image provider and creating stream %d x %d",
               streamWidth, streamHeight);
        provider = createImgProvider(streamWidth, streamHeight, 2, VDO_FORMAT_YUV);
        if (!provider) {
            syslog(LOG_ERR, "%s: Failed to create ImgProvider", __func__);
            exit(2);
        }

        syslog(LOG_INFO, "Start fetching video frames from VDO");
        if (!startFrameFetch(provider)) {
            syslog(LOG_ERR, "%s: Failed to fetch frames from VDO", __func__);
            exit(3);
        }
        successInitialize = true;
    }



    // Create OpenCV Mats for the camera frame (nv12), the converted frame (bgr)
    // and the foreground frame that is outputted by the background subtractor
    bgr_mat = Mat(height, width, CV_8UC3);
    Mat nv12_mat = Mat(height * 3 / 2, width, CV_8UC1);
    Mat fg;
    Mat img;

    std::vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};
    std::string boxString = "";
    syslog(LOG_INFO,"Running loop");

    for(int i = 0; i < times || times == -1; i++) {

        try {

            boxString = "{";

            if(finishedThread){
                return;
            }

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            // Get the latest NV12 image frame from VDO using the imageprovider
            VdoBuffer* buf = getLastFrameBlocking(provider);
            if (!buf) {
                syslog(LOG_INFO, "No more frames available, exiting");
                exit(0);
            }
            if(times == -1){
                syslog(LOG_INFO,"Running %d-INF step of the loop",i+1);
            }else{
                syslog(LOG_INFO,"Running %d-%d step of the loop",i+1,times);
            }
            syslog(LOG_INFO,"Converting NV12 to BGR");
            // Assign the VDO image buffer to the nv12_mat OpenCV Mat.
            // This specific Mat is used as it is the one we created for NV12,
            // which has a different layout than e.g., BGR.
            nv12_mat.data = static_cast<uint8_t*>(vdo_buffer_get_data(buf));

            // Convert the NV12 data to BGR
            cvtColor(nv12_mat, bgr_mat, COLOR_YUV2BGR_NV12, 3);

            cv::resize(bgr_mat, img, cv::Size(resize_width, resize_height));
            cv::cvtColor(img, img, cv::COLOR_BGR2RGB);  // BGR -> RGB
            img.convertTo(img, CV_32FC3, 1.0f / 255.0f);  // normalization 1/255
            auto imgTensor = torch::from_blob(img.data, { 1, img.rows, img.cols, img.channels() }).to(device_type);
            imgTensor = imgTensor.permute({ 0, 3, 1, 2 }).contiguous();  // BHWC -> BCHW (Batch, Channel, Height, Width)
            std::vector<torch::jit::IValue> inputs;
            inputs.emplace_back(imgTensor);

            torch::jit::IValue output = module.forward(inputs);
            auto preds = output.toTensor();
            //auto preds = output.toTuple()->elements()[0].toTensor();
            std::vector<torch::Tensor> dets = non_max_suppression(preds, 0.25, 0.45);

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            std::string timeInString = std::to_string((std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0);

            time_t now;
            time(&now);
            char timeBuf[256];
            strftime(timeBuf, sizeof timeBuf, "%FT%TZ", gmtime(&now));

            boxString += "\"extime\":" + timeInString
                         + ",\"timestamp\":\""+timeBuf+"\",\"results\":[";

            std::string messageToHtmlPageBoxOutput = "[";

            if (dets.size() > 0)
            {
                // Visualize result
                for (size_t i = 0; i < dets[0].sizes()[0]; ++i)
                {
                    float left = dets[0][i][0].item().toFloat() * bgr_mat.cols / resize_width;
                    float top = dets[0][i][1].item().toFloat() * bgr_mat.rows / resize_height;
                    float right = dets[0][i][2].item().toFloat() * bgr_mat.cols / resize_width;
                    float bottom = dets[0][i][3].item().toFloat() * bgr_mat.rows / resize_height;
                    float score = dets[0][i][4].item().toFloat();
                    int classID = dets[0][i][5].item().toInt();

                    cv::rectangle(bgr_mat, cv::Rect(left, top, (right - left), (bottom - top)), colors[classID], 3);

                    cv::putText(bgr_mat,
                                classnames[classID] + ": " + cv::format("%.2f", score),
                                cv::Point(left, top),
                                cv::FONT_HERSHEY_SIMPLEX, (right - left) / 200, colors[classID], 2);

                    boxString += "{\"bbox\":[" + std::to_string(left) + "," + std::to_string(top)
                                + "," + std::to_string(right) + "," +
                                std::to_string(bottom) +
                                "],\"confidence\":" + std::to_string(score) + ",\"class\":\"" + classnames[classID] + "\"},";

                    messageToHtmlPageBoxOutput += "["+std::to_string(left)+
                            ","+std::to_string(top)+","+std::to_string(right)+
                            ","+std::to_string(bottom)+","+std::to_string(score)+","+classnames[classID]+"],";

                    //syslog(LOG_INFO,"%f",(std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count()) /1000000.0 );
                    if(!finishedThread) {
                        syslog(LOG_INFO, "[%s,%f,%f,%f,%f,%f,%s]",
                               timeInString.c_str(),
                               left, top, right, bottom, score,
                               classnames[classID].c_str());
                    }
                }
            }else{

                if(!finishedThread) {
                    syslog(LOG_INFO, "[%s]",
                           timeInString.c_str());
                }
            }

            if(boxString[boxString.size()-1] == ','){
                boxString.pop_back();
            }

            boxString += "]}";

            if(messageToHtmlPageBoxOutput[messageToHtmlPageBoxOutput.size()-1] == ',') {
                messageToHtmlPageBoxOutput.pop_back();
            }

            messageToHtmlPageBoxOutput += "]";



            if(mqtt_on_value == "on" && !finishedThread){
                readConfigFile();
                sendMessageMQTT(boxString);
            }

            std::vector<uchar> buff_bgr;
            cv::imencode(".jpg", bgr_mat, buff_bgr, params);
            //streamer.publish("/bgr", std::string(buff_bgr.begin(), buff_bgr.end()));
            unsigned char* base64_jpg = reinterpret_cast<unsigned char*>(buff_bgr.data());


            currentImage = "\"data:image/jpeg;base64," + base64_encode(base64_jpg, buff_bgr.size()) + "\"";
            currentBox = "\"<div class=\\\"msg\\\" id=\\\"msg_" + std::to_string(i+1) +"\\\">" +
                    std::to_string(i+1) + "/" + std::to_string(times) + "<br>" +
                    "Time: " + timeInString + "<br>" +
                    "Results: " + messageToHtmlPageBoxOutput + "<br><br></div>"
                     + "\"";


            if(!finishedThread){
                syslog(LOG_INFO, "Sleeping %d ms", stoi(sleepTime));
                usleep(stoi(sleepTime) * 1000);
            }
    //        Ptr<Formatted> fmtd = format(bgr_mat,Formatter::FMT_NUMPY);
    //
    //        fmtd->reset();
    //
    //        //for(const char* str = fmtd->next(); str; str = fmtd->next())
    //        //    syslog(LOG_INFO, "%s", str );
    //
    //        int size = 0;
    //        char* logFrame = new char[256];
    //        memset(logFrame, 0, 100);
    //        for (const char* str = fmtd->next(); str; str = fmtd->next()) {
    //            size += strlen(str);
    //            strcat(logFrame, str);
    //
    //            if (size > 100) {
    //                syslog(LOG_INFO,"%s", logFrame);
    //                size = 0;
    //                memset(logFrame, 0, 100);
    //            }
    //        }

    //        delete logFrame;
    //        logFrame = NULL;

            // Release the VDO frame buffer
            returnFrame(provider, buf);
        }
        catch (const c10::Error& e) {
            syslog(LOG_INFO, "error during running this loop");
        }
    }

    finishedThread = true;
    syslog(LOG_INFO,"Execution Model FINISHED");
}

enum {
    bufsize = 16384
};
static char buf[bufsize];

bool replace(std::string& str, const std::string& from, const std::string& to) {
    size_t start_pos = str.find(from);
    if(start_pos == std::string::npos)
        return false;
    str.replace(start_pos, from.length(), to);
    return true;
}


static void front(std::string customScript = "", std::string executeName = "execute",
                  std::string executeValue = "Execute",
                  std::string buttonStyle = "none",
                  std::string inputExecutionTimesStatus = "") {


    if(finishedThread == false){
        executeName = "stopThread";
        executeValue = "Stop";
        buttonStyle = "block";
        inputExecutionTimesStatus = "disabled";
    }

    readConfigFile();

    std::ifstream mainFile("/usr/local/packages/disit_app/html/main.html");
    std::string str;
    std::string file_contents;
    while (std::getline(mainFile, str))
    {
        file_contents += str;
        file_contents.push_back('\n');
    }

    mainFile.close();

    replace(file_contents, "${ADDRESS}", address);
    replace(file_contents, "${PORT}", port);
    replace(file_contents, "${TOPIC_NAME}", topic_name);
    replace(file_contents, "${CLIENT_ID}", client_id);
    replace(file_contents, "${QOS}", qos);
    replace(file_contents, "${TIMEOUT}", mqtt_timeout);

    replace(file_contents, "${EXECUTE_TIMES_ON_STARTUP}", times_execution_onstartup);
    replace(file_contents, "${SLEEP}", sleepTime);
    replace(file_contents, "${CUSTOM_SCRIPT}", customScript);
    if(mqtt_on_value == "on"){
        replace(file_contents, "${MQTT_ON}", "checked");
    }else if(mqtt_on_value == "off"){
        replace(file_contents, "${MQTT_ON}", "");
    }

    replace(file_contents, "${EXECUTE_NAME}", executeName);
    replace(file_contents, "${EXECUTE_VALUE}", executeValue);
    replace(file_contents, "${BUTTON_RESULT_STYLE}", buttonStyle);
    replace(file_contents,"${INPUT_EXECUTION_TIMES_STATUS}",inputExecutionTimesStatus);

    sprintf(buf,"%s",file_contents.c_str());

}

std::string UrlDecode(const std::string& value)
{
    std::string result;
    result.reserve(value.size());

    for (std::size_t i = 0; i < value.size(); ++i)
    {
        auto ch = value[i];

        if (ch == '%' && (i + 2) < value.size())
        {
            auto hex = value.substr(i + 1, 2);
            auto dec = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
            result.push_back(dec);
            i += 2;
        }
        else if (ch == '+')
        {
            result.push_back(' ');
        }
        else
        {
            result.push_back(ch);
        }
    }

    return result;
}


std::vector<std::string> explode(std::string const & s, char delim)
{
    std::vector<std::string> result;
    std::istringstream iss(s);

    for (std::string token; std::getline(iss, token, delim); )
    {
        result.push_back(std::move(token));
    }

    return result;
}

std::string processPost(const char *post, std::string customScript = ""){
    syslog(LOG_INFO,"%s",post);
    std::string strPost(post);
    std::string decodedPost = UrlDecode(strPost);
    std::vector<std::string> params = explode(decodedPost,'&');
    syslog(LOG_INFO,"%s",decodedPost.c_str());

    for(auto s : params){
        std::vector<std::string> keyValue = explode(s,'=');

        if(keyValue[0] == "address"){
            address = keyValue[1];
        }else if(keyValue[0] == "port"){
            port = keyValue[1];
        }else if(keyValue[0] == "topic_name"){
            topic_name = keyValue[1];
        }else if(keyValue[0] == "client_id"){
            client_id = keyValue[1];
        }else if(keyValue[0] == "qos"){
            qos = keyValue[1];
        }else if(keyValue[0] == "timeout"){
            mqtt_timeout = keyValue[1];
        }else if(keyValue[0] == "execution_times_on_startup"){
            times_execution_onstartup = keyValue[1];
        }else if(keyValue[0] == "sleep"){
            sleepTime = keyValue[1];
        }else if(keyValue[0] == "mqtt_on"){
            mqtt_on_value = keyValue[1];
        }else if(keyValue[0] == "execution_times"){
            syslog(LOG_INFO,"Times %s : %s",keyValue[0].c_str(),keyValue[1].c_str());
            return keyValue[1];
        }

        syslog(LOG_INFO,"%s : %s",keyValue[0].c_str(),keyValue[1].c_str());
    }

    saveSettings();

    return "";
}


int list_2(const mklib_session *sr, const char *vhost, const char *url,
                  const char *get, unsigned long getlen,
                  const char *post, unsigned long postlen,
                  unsigned int *status, const char **content, unsigned long *content_len,
                  char *header) {

   // if (strcmp(url, "/image.png") == 0) {
   //     *content = (char *) monkey_head_png;
   //     *content_len = sizeof(monkey_head_png);
   //     sprintf(header, "Content-type: image/png");

   //     return MKLIB_TRUE;
   // }

    if(strcmp(url,"/results") == 0){
        std::ifstream mainFile("/usr/local/packages/disit_app/html/results.html");
        std::string str;
        std::string file_contents;
        while (std::getline(mainFile, str))
        {
            file_contents += str;
            file_contents.push_back('\n');
        }

        mainFile.close();
        sprintf(buf,"%s",file_contents.c_str());
        *content = buf;
        *content_len = strlen(buf);
        sprintf(header, "Content-type: text/html");
        return MKLIB_TRUE;
    }

    if (!post) {
        front();
    } else {

        if(strstr(post,"ajax=true") && strstr(post,"image=true")){
            //ajaxProcess(images,content,content_len, header);

            *content = currentImage.c_str();
            *content_len = currentImage.size();

            sprintf(header, "Content-type: application/json");
            return MKLIB_TRUE;
        }

        if(strstr(post,"ajax=true") && strstr(post,"box=true")){

            *content = currentBox.c_str();
            *content_len = currentBox.size();

            sprintf(header, "Content-type: application/json");
            return MKLIB_TRUE;
        }

        if(strstr(post,"stopThread=Stop")){
            if(!finishedThread) {
                syslog(LOG_INFO, "Previous thread stopping...");
                finishedThread = true;
                syslog(LOG_INFO, "Thread successufly stopped by the user");
            }else{
                syslog(LOG_INFO, "Thread is already finished...");
            }


            front();
        }

        if (strstr(post, "save=Save") || strstr(post,"saveBasicSetting=Save")){
            processPost(post);
            front("<script>swal(\"Successfully saved!\", \"\", \"success\");</script>");

        }

        if(strstr(post,"execute=Execute")){
            signed int times = stoi(processPost(post));

            syslog(LOG_INFO,"Creating thread motion detector with %d execution",times);
            t1 = std::thread(motion_detector,times);
            t1.detach();
            front("<script>swal(\"Successfully sent execution command!\", \"thread is created, it is executing "+std::to_string(times)+" times\", \"success\");</script>"
            ,"stopThread","Stop","block","disabled");
        }



    }

    *content = buf;
    *content_len = strlen(buf);
    sprintf(header, "Content-type: text/html");

    return MKLIB_TRUE;
}




int main(int argc, char* argv[]) {

    //streamer.start(8000);
    GMainLoop *loop;
    openlog("disit_app", LOG_PID|LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "starting  %s", argv[0]);
    loop = g_main_loop_new(NULL, FALSE);

    readConfigFile();

    mklib_ctx ctx = mklib_init(NULL, 0, 0, NULL);
    if (!ctx) {
        syslog(LOG_INFO, "mklib_init failed");
        exit(0);
    }
    syslog(LOG_INFO, "mklib_init success");

    mklib_callback_set(ctx, MKCB_DATA,(void*) listf);
    syslog(LOG_INFO, "mklib_callback_set success");

    // Start the server.
    mklib_start(ctx);
    syslog(LOG_INFO, "mklib_start success");

    thread t2(motion_detector,stoi(times_execution_onstartup));
    t2.detach();

    g_main_loop_run(loop);

    mklib_stop(ctx);
    //streamer.stop();

    closelog();


    return EXIT_SUCCESS;
}