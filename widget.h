#ifndef WIDGET_H
#define WIDGET_H

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Float32MultiArray.h>
#include <std_msgs/Float64.h>
#include <geometry_msgs/Twist.h>
#include <nav_msgs/Odometry.h>
#include <std_msgs/Int8.h>
#include <can_msgs/Frame.h>

#include <iostream>

#include <QTimer>
#include <QWidget>

#include<iostream>

#define PI 3.1415926
#define DISTANCE_WHEELS  0.562
#define DIAMETER_WHEELS    0.169
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
//--------------------ros_workers-------------------------------
    std::shared_ptr<ros::NodeHandle> ptr_nh_;

    ros::Subscriber receive_data_sub_;
    ros::Publisher work_mdl_pub_;
    ros::Publisher motor_speed_pub_;
    ros::Publisher data_goal_rps_pub_;
    ros::Publisher data_real_rps_pub_;
    ros::Publisher data_goal_angluar_pub_;
    ros::Publisher real_twist_pub_;

//---------------------QT_object--------------------------
    QTimer timer_display_;

//--------------------static_data-------------------------
    static double speed_rpm_[2];


//---------------------call_back--------------------------
    static void receivedMsgCB(const can_msgs::Frame & msg);

//----------------------flag-----------------------------
    static bool flag_read_motor_;

//----------------------data------------------------------
    double goal_speed_rpm_[2];
private slots:
    void on_pushButton_clicked();

    void time_up_display();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
