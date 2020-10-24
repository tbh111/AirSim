//// Copyright (c) Microsoft Corporation. All rights reserved.
//// Licensed under the MIT License.
//
//#include "common/common_utils/StrictMode.hpp"
//STRICT_MODE_OFF
//#ifndef RPCLIB_MSGPACK
//#define RPCLIB_MSGPACK clmdep_msgpack
//#endif // !RPCLIB_MSGPACK
//#include "rpc/rpc_error.h"
//STRICT_MODE_ON
//
//#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
//#include "common/common_utils/FileSystem.hpp"
//#include <iostream>
//#include <chrono>
//
//int main() 
//{
//    using namespace msr::airlib;
//
//    msr::airlib::MultirotorRpcLibClient client;
//    typedef ImageCaptureBase::ImageRequest ImageRequest;
//    typedef ImageCaptureBase::ImageResponse ImageResponse;
//    typedef ImageCaptureBase::ImageType ImageType;
//    typedef common_utils::FileSystem FileSystem;
//    
//    try {
//        client.confirmConnection();
//
//        std::cout << "Press Enter to get FPV image" << std::endl; std::cin.get();
//        vector<ImageRequest> request = { ImageRequest("0", ImageType::Scene), ImageRequest("1", ImageType::DepthPlanner, true) };
//        const vector<ImageResponse>& response = client.simGetImages(request);
//        std::cout << "# of images received: " << response.size() << std::endl;
//
//        if (response.size() > 0) {
//            std::cout << "Enter path with ending separator to save images (leave empty for no save)" << std::endl; 
//            std::string path;
//            std::getline(std::cin, path);
//
//            for (const ImageResponse& image_info : response) {
//                std::cout << "Image uint8 size: " << image_info.image_data_uint8.size() << std::endl;
//                std::cout << "Image float size: " << image_info.image_data_float.size() << std::endl;
//
//                if (path != "") {
//                    std::string file_path = FileSystem::combine(path, std::to_string(image_info.time_stamp));
//                    if (image_info.pixels_as_float) {
//                        Utils::writePfmFile(image_info.image_data_float.data(), image_info.width, image_info.height,
//                            file_path + ".pfm");
//                    }
//                    else {
//                        std::ofstream file(file_path + ".png", std::ios::binary);
//                        file.write(reinterpret_cast<const char*>(image_info.image_data_uint8.data()), image_info.image_data_uint8.size());
//                        file.close();
//                    }
//                }
//            }
//        }
//
//        std::cout << "Press Enter to arm the drone" << std::endl; std::cin.get();
//        client.enableApiControl(true);
//        client.armDisarm(true);
//
//        auto barometer_data = client.getBarometerData();
//        std::cout << "Barometer data \n" 
//            << "barometer_data.time_stamp \t" << barometer_data.time_stamp << std::endl
//            << "barometer_data.altitude \t" << barometer_data.altitude << std::endl
//            << "barometer_data.pressure \t" << barometer_data.pressure << std::endl
//            << "barometer_data.qnh \t" << barometer_data.qnh << std::endl;
//
//        auto imu_data = client.getImuData();
//        std::cout << "IMU data \n" 
//            << "imu_data.time_stamp \t" << imu_data.time_stamp << std::endl 
//            << "imu_data.orientation \t" << imu_data.orientation << std::endl 
//            << "imu_data.angular_velocity \t" << imu_data.angular_velocity << std::endl 
//            << "imu_data.linear_acceleration \t" << imu_data.linear_acceleration << std::endl;
//
//        auto gps_data = client.getGpsData();
//        std::cout << "GPS data \n" 
//            << "gps_data.time_stamp \t" << gps_data.time_stamp << std::endl 
//            << "gps_data.gnss.time_utc \t" << gps_data.gnss.time_utc << std::endl
//            << "gps_data.gnss.geo_point \t" << gps_data.gnss.geo_point << std::endl
//            << "gps_data.gnss.eph \t" << gps_data.gnss.eph << std::endl
//            << "gps_data.gnss.epv \t" << gps_data.gnss.epv << std::endl
//            << "gps_data.gnss.velocity \t" << gps_data.gnss.velocity << std::endl
//            << "gps_data.gnss.fix_type \t" << gps_data.gnss.fix_type << std::endl;
//
//        auto magnetometer_data = client.getMagnetometerData();
//        std::cout << "Magnetometer data \n" 
//            << "magnetometer_data.time_stamp \t" << magnetometer_data.time_stamp << std::endl 
//            << "magnetometer_data.magnetic_field_body \t" << magnetometer_data.magnetic_field_body << std::endl; 
//            // << "magnetometer_data.magnetic_field_covariance" << magnetometer_data.magnetic_field_covariance // not implemented in sensor
//
//        std::cout << "Press Enter to takeoff" << std::endl; std::cin.get();
//        float takeoffTimeout = 5; 
//        client.takeoffAsync(takeoffTimeout)->waitOnLastTask();
//
//        // switch to explicit hover mode so that this is the fall back when 
//        // move* commands are finished.
//        std::this_thread::sleep_for(std::chrono::duration<double>(5));
//        client.hoverAsync()->waitOnLastTask();
//
//        std::cout << "Press Enter to fly in a 10m box pattern at 3 m/s velocity" << std::endl; std::cin.get();
//        // moveByVelocityZ is an offboard operation, so we need to set offboard mode.
//        client.enableApiControl(true); 
//        auto position = client.getMultirotorState().getPosition();
//        float z = position.z(); // current position (NED coordinate system).  
//        const float speed = 3.0f;
//        const float size = 10.0f; 
//        const float duration = size / speed;
//        DrivetrainType driveTrain = DrivetrainType::ForwardOnly;
//        YawMode yaw_mode(true, 0);
//        std::cout << "moveByVelocityZ(" << speed << ", 0, " << z << "," << duration << ")" << std::endl;
//        client.moveByVelocityZAsync(speed, 0, z, duration, driveTrain, yaw_mode);
//        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
//        std::cout << "moveByVelocityZ(0, " << speed << "," << z << "," << duration << ")" << std::endl;
//        client.moveByVelocityZAsync(0, speed, z, duration, driveTrain, yaw_mode);
//        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
//        std::cout << "moveByVelocityZ(" << -speed << ", 0, " << z << "," << duration << ")" << std::endl;
//        client.moveByVelocityZAsync(-speed, 0, z, duration, driveTrain, yaw_mode);
//        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
//        std::cout << "moveByVelocityZ(0, " << -speed << "," << z << "," << duration << ")" << std::endl;
//        client.moveByVelocityZAsync(0, -speed, z, duration, driveTrain, yaw_mode);
//        std::this_thread::sleep_for(std::chrono::duration<double>(duration));
//
//        client.hoverAsync()->waitOnLastTask();
//
//        std::cout << "Press Enter to land" << std::endl; std::cin.get();
//        client.landAsync()->waitOnLastTask();
//
//        std::cout << "Press Enter to disarm" << std::endl; std::cin.get();
//        client.armDisarm(false);
//
//    }
//    catch (rpc::rpc_error&  e) {
//        std::string msg = e.get_error().as<std::string>();
//        std::cout << "Exception raised by the API, something went wrong." << std::endl << msg << std::endl;
//    }
//
//    return 0;
//}
#include <iostream>
#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "api/RpcLibClientBase.hpp"
#include <opencv.hpp>
#include <core.hpp>
#include <imgproc.hpp>
#include <imgcodecs.hpp>
#include <highgui.hpp>
#include <stitching.hpp>
#include <vector>
#include "common/common_utils/FileSystem.hpp"
#include <conio.h>
using namespace cv;
using namespace msr::airlib;
using namespace std;

void getOneImage(MultirotorRpcLibClient& Client)
{
    //// for car use CarRpcLibClient
    ////MultirotorRpcLibClient client;
    //Mat tres;
    //Mat output;
    //typedef ImageCaptureBase::ImageType ImageType;
    //std::vector<uint8_t> png_image = Client.simGetImage("0",ImageType::Scene);
    //// do something with images
    //tres = Mat(png_image, true);//vector2mat
    //transpose(tres, output);
    //cout << "1" << endl;
    ////output = output.reshape(1, 768);
    //
    //cout << "cols:" << tres.cols << endl;
    //cout << "rows" << tres.rows << endl;
    //cout << "type" << tres.type() << endl;
    //cout << "channels" << tres.channels() << endl;
    //std::cout << "Press Enter to get FPV image" << std::endl; std::cin.get();
    typedef ImageCaptureBase::ImageRequest ImageRequest;
    typedef ImageCaptureBase::ImageResponse ImageResponse;
    typedef ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;
    std::vector<ImageRequest> request = { ImageRequest("0", ImageType::Scene) };
    const std::vector<ImageResponse>& response = Client.simGetImages(request);
    std::cout << "# of images received: " << response.size() << std::endl;

    if (response.size() > 0) {
        std::cout << "Enter path with ending separator to save images (leave empty for no save)" << std::endl;
        std::string path;
        std::getline(std::cin, path);
        path = "E:/drone/pic";
        for (const ImageResponse& image_info : response) {
            std::cout << "Image uint8 size: " << image_info.image_data_uint8.size() << std::endl;
            std::cout << "Image float size: " << image_info.image_data_float.size() << std::endl;

            if (path != "") {
                std::string file_path = FileSystem::combine(path, std::to_string(image_info.time_stamp));
                if (image_info.pixels_as_float) {
                    Utils::writePfmFile(image_info.image_data_float.data(), image_info.width, image_info.height,
                        file_path + ".pfm");
                }
                else {
                    std::ofstream file(file_path + ".png", std::ios::binary);
                    file.write(reinterpret_cast<const char*>(image_info.image_data_uint8.data()), image_info.image_data_uint8.size());
                    file.close();
                }
            }
        }
        //return output;
    }
}

int main()
{
    using namespace std;
    MultirotorRpcLibClient client;

    cout << "Press Enter to enable API control" << endl; cin.get();
    client.enableApiControl(true);

    cout << "Press Enter to arm the drone" << endl; cin.get();
    client.armDisarm(true);

    cout << "Press Enter to takeoff" << endl; cin.get();
    client.takeoffAsync(5)->waitOnLastTask();
    char a;
    while (1) {
        cout << "Press Enter to move 5 meters in x direction with 1 m/s velocity" << endl; 
        a = getch();
        if (a == 'w') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            //client.moveToPositionAsync(position.x() + 5, position.y(), position.z(), 1)->waitOnLastTask();
            client.moveToPositionAsync(position.x() + 5, position.y(), position.z(), 1);
        }
        else if (a == 'a') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            client.moveToPositionAsync(position.x(), position.y() - 5, position.z(), 1);
        }
        else if (a == 's') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            client.moveToPositionAsync(position.x() - 5, position.y(), position.z(), 1);
        }
        else if (a == 'd') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            client.moveToPositionAsync(position.x(), position.y() + 5, position.z(), 1);
        }
        else if (a == 'q') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            client.moveToPositionAsync(position.x(), position.y(), position.z() - 5, 1);
        }
        else if (a == 'e') {
            auto position = client.getMultirotorState().getPosition(); // from current location
            client.moveToPositionAsync(position.x(), position.y(), position.z() + 5, 1);
        }
        else if (a == 'r') {
            //auto position = client.getMultirotorState().getPosition(); // from current location
            //client.moveToPositionAsync(position.x(), position.y(), position.z() + 5, 1)->waitOnLastTask();
            getOneImage(client);
            //typedef ImageCaptureBase::ImageType ImageType;
            //std::vector<uint8_t> png_image = client.simGetImage("0", ImageType::Scene);
            //// do something with images
            //tres = Mat(png_image, true);//vector2mat
            
        }
    }

    cout << "Press Enter to land" << endl; cin.get();
    client.landAsync()->waitOnLastTask();

    return 0;
}