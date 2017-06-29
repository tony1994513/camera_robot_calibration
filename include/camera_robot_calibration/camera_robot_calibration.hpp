#ifndef _BAXTER_MOVER_HPP
#define _BAXTER_MOVER_HPP

#include "parameters.hpp"
#include "helpers_methods.hpp"

namespace camera_robot_calibration {
Data_config _global_parameters;
class ROBOT;
class CAMERA;


class CAMERA{
public:
    typedef std::shared_ptr<CAMERA> Ptr;
    typedef const std::shared_ptr<CAMERA> ConstPtr;

//    CAMERA(Data_config& parameters){
//        init(parameters);
//    }

//    virtual void init(Data_config& parameters) = 0;


    std::shared_ptr<rgbd_utils::RGBD_Subscriber> _syncronized_camera_sub;
protected:
    ros::NodeHandle _nh_camera;
    std::unique_ptr<ros::AsyncSpinner> _camera_spinner;
    ros::Subscriber _camera_topics_sub;
};

class CAMERA_kinect_freenect : public CAMERA{
public:
    typedef std::shared_ptr<CAMERA_kinect_freenect> Ptr;
    typedef const std::shared_ptr<CAMERA_kinect_freenect> ConstPtr;

    CAMERA_kinect_freenect(){

    }

    void init(Data_config &parameters);
    void camera_topics_start_publishing_cb(const sensor_msgs::Image::ConstPtr& depth_msgs){
        _global_parameters.set_camera_topics_status(true);
    }
    ros::NodeHandle _freenect_nh;
    std::shared_ptr<rgbd_utils::RGBD_Subscriber> _syncronized_freenect_sub;
};

class CAMERA_kinect_openni : public CAMERA{
public:
    typedef std::shared_ptr<CAMERA_kinect_openni> Ptr;
    typedef const std::shared_ptr<CAMERA_kinect_openni> ConstPtr;

    CAMERA_kinect_openni(){

    }

    void init(Data_config &parameters);
    void camera_topics_start_publishing_cb(const sensor_msgs::Image::ConstPtr& depth_msgs){
        _global_parameters.set_camera_topics_status(true);
    }
};

class CAMERA_kinect_v2 : public CAMERA{
public:
    typedef std::shared_ptr<CAMERA_kinect_v2> Ptr;
    typedef const std::shared_ptr<CAMERA_kinect_v2> ConstPtr;

    CAMERA_kinect_v2(){

    }

    void init(Data_config &parameters);
    void camera_topics_start_publishing_cb(const sensor_msgs::Image::ConstPtr& depth_msgs){
        _global_parameters.set_camera_topics_status(true);
    }

};

class ROBOT{
public:
    typedef std::shared_ptr<ROBOT> Ptr;
    typedef const std::shared_ptr<ROBOT> ConstPtr;

//    ROBOT(){
//        init();
//    }

//    virtual void init() = 0;

protected:
    ros::NodeHandle _nh_robot;
    std::unique_ptr<ros::AsyncSpinner> _robot_spinner;
    std::shared_ptr<ros::Subscriber> _eef_state_sub;
};

class BAXTER : public ROBOT {
public:
    typedef std::shared_ptr<BAXTER> Ptr;
    typedef const std::shared_ptr<BAXTER> ConstPtr;

    BAXTER(){}

    void init();

    //call back that register baxter left end effector pose and rearrange the orientation in RPY
    void eef_Callback(const baxter_core_msgs::EndpointState::ConstPtr& eef_feedback){
        calibration_helpers_methods::locate_eef_pose(eef_feedback->pose, _global_parameters);
        //ROS_WARN_STREAM("locating eef stuff gave for position: " << data_simu.get_eef_position()
                    //    << "\n and for orientation: " << data_simu.get_eef_rpy_orientation());
    }
};

class CRUSTCRAWLER : public ROBOT{
public:
    typedef std::shared_ptr<CRUSTCRAWLER> Ptr;
    typedef const std::shared_ptr<CRUSTCRAWLER> ConstPtr;

    CRUSTCRAWLER(){}
    void init();

    //call back that register baxter left end effector pose and rearrange the orientation in RPY
    void eef_Callback(const crustcrawler_core_msgs::EndpointState::ConstPtr& eef_feedback){
        calibration_helpers_methods::locate_eef_pose(eef_feedback->pose, _global_parameters);
        //ROS_WARN_STREAM("locating eef stuff gave for position: " << data_simu.get_eef_position()
                    //    << "\n and for orientation: " << data_simu.get_eef_rpy_orientation());
    }
};

class CALIBRATOR{
public:
    typedef std::shared_ptr<CALIBRATOR> Ptr;
    typedef const std::shared_ptr<CALIBRATOR> ConstPtr;

    CALIBRATOR(){
        init();
    }

    void init();

    void acquire_points();
    void solve_for_transformation_matrix();

    Data_config& get_global_parameters(){
        return _global_parameters;
    }

    friend class CAMERA;
    friend class ROBOT;



private:
    ros::NodeHandle _nh_clibrator;
    std::unique_ptr<ros::AsyncSpinner> _calibrator_spinner;
    ROBOT::Ptr _robot;
    CAMERA::Ptr _camera;
    CAMERA_kinect_freenect _test_camera;
};

}

#endif //_BAXTER_MOVER_HPP
