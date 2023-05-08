#include "widget.h"
#include "ui_widget.h"
nav_msgs::Odometry msg_odom;
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ptr_nh_ = std::make_shared<ros::NodeHandle>();

    receive_data_sub_ = ptr_nh_->subscribe("/received_messages",1000,receivedMsgCB);

    work_mdl_pub_ =
            ptr_nh_->advertise<std_msgs::Int8>("/work_mdl_shift", 1000);
    motor_speed_pub_
            = ptr_nh_->advertise<std_msgs::Float32MultiArray>("/motor_speed", 1000);

    data_goal_rps_pub_ =
            ptr_nh_->advertise<std_msgs::Float64>("/goal_speed_rps", 1000);
    data_real_rps_pub_ =
            ptr_nh_->advertise<std_msgs::Float64>("/real_speed_rps", 1000);
    data_goal_angluar_pub_ =
            ptr_nh_->advertise<std_msgs::Float64>("/real_speed_angluar_yaw", 1000);

    real_twist_pub_ =
            ptr_nh_->advertise<nav_msgs::Odometry>("/real_twist", 1000);



    static ros::AsyncSpinner spinner(1);
    spinner.start();

    connect(&timer_display_,SIGNAL(timeout()),this,SLOT(time_up_display()));
    timer_display_.start(20);
}

double Widget::speed_rpm_[2] = {0.0 , 0.0};
bool Widget::flag_read_motor_ = false;
Widget::~Widget()
{
    delete ui;
}

void Widget::receivedMsgCB(const can_msgs::Frame & msg)
{
    int bit = 24;
    int dec_data = 0;


    for(int i = 7 ;i >= 4 ; i --)
    {
        dec_data += msg.data.at(i) << bit;
        bit -= 8;
    }

    double dec_tmp = static_cast<double>(dec_data);
    double encoder_resolution = 10000;


    //右轮：
    if(msg.id == 0x181)
    {
        speed_rpm_[0] = dec_tmp / 512.0 / encoder_resolution * 1875;
    }

    //右轮：
    if(msg.id == 0x182)
    {
        speed_rpm_[1] = dec_tmp / 512.0 / encoder_resolution * 1875;
    }

    flag_read_motor_ = true;

}

//----------------------------slot---------------------------------------------

void Widget::on_pushButton_clicked()
{
    std_msgs::Float32MultiArray msg;
    msg.data.clear();
    if(ui->comboBox->currentIndex() == 0)
    {
        msg.data.push_back(-1);
        msg.data.push_back(ui->lineEdit->text().toDouble());
        goal_speed_rpm_[0] = ui->lineEdit->text().toDouble();
    }
    else if(ui->comboBox->currentIndex() == 1)
    {
        msg.data.push_back(1);
        msg.data.push_back(ui->lineEdit_2->text().toDouble());
        goal_speed_rpm_[1] = ui->lineEdit_2->text().toDouble();
    }
    else if(ui->comboBox->currentIndex() == 2)
    {
        msg.data.push_back(2);
        msg.data.push_back(ui->lineEdit->text().toDouble());
        msg.data.push_back(ui->lineEdit_2->text().toDouble());
        goal_speed_rpm_[0] = ui->lineEdit->text().toDouble();
        goal_speed_rpm_[1] = ui->lineEdit_2->text().toDouble();
    }

    motor_speed_pub_.publish(msg);

}



void Widget::time_up_display()
{
//    ui->lcd_l_speed->display(speed_rpm_[0] / 9.55 * 180 / PI);
//    ui->lcd_r_speed->display(speed_rpm_[1] / 9.55 * 180 / PI);
    ui->lcd_l_speed->display(speed_rpm_[0] / 60.0);
    ui->lcd_r_speed->display(speed_rpm_[1] / 60.0);


    static double min_cnt = 0x3f3f3f3f;
    static double max_cnt = -0x3f3f3f3f;



    std_msgs::Float64 msg_real;
    msg_real.data = speed_rpm_[1] / 60.0;
    data_real_rps_pub_.publish(msg_real);

    std_msgs::Float64 msg_goal;
    msg_goal.data = goal_speed_rpm_[1] / 60.0;
    data_goal_rps_pub_.publish(msg_goal);

    double l_rps = speed_rpm_[0] / 60.0;
    double r_rps = speed_rpm_[1] / 60.0;
    double v_left = l_rps * DIAMETER_WHEELS * PI;
    double v_right = -r_rps * DIAMETER_WHEELS * PI;



    msg_odom.header.stamp = ros::Time::now();
    msg_odom.twist.twist.linear.x = (v_left + v_right) /2.0 /(2.0 * PI);
    msg_odom.pose.pose.position.x += msg_odom.twist.twist.linear.x*0.02;
    msg_odom.twist.twist.angular.z = (v_right - v_left) / 2.0 / DISTANCE_WHEELS / PI;
    real_twist_pub_.publish(msg_odom);

    std_msgs::Float64 msg_goal_yaw;
    l_rps = goal_speed_rpm_[0] / 60.0;
    r_rps = goal_speed_rpm_[1] / 60.0;
    std::cout << l_rps << "   " << r_rps << std::endl;
    v_left = l_rps * DIAMETER_WHEELS * PI;
    v_right = r_rps * DIAMETER_WHEELS * PI;
    msg_goal_yaw.data = (v_right - v_left) / 2.0 / DISTANCE_WHEELS / PI;
    data_goal_angluar_pub_.publish(msg_goal_yaw);

    if(flag_read_motor_ == true)
    {
        if(min_cnt >  msg_odom.twist.twist.linear.x)
        {
            min_cnt =  msg_odom.twist.twist.linear.x;
        }
        if(max_cnt < msg_odom.twist.twist.linear.x)
        {
            max_cnt =  msg_odom.twist.twist.linear.x;
        }
    }


    std::cout << min_cnt << " ~ " << max_cnt << std::endl;
}
