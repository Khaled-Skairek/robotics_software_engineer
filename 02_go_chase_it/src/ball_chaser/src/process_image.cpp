#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>


class ImageProcessor
{
public:
	ImageProcessor()
	{
		command_robot_service_client_ = n_.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");
		camera_images_subscriber_ = n_.subscribe("/camera/rgb/image_raw", 10, &ImageProcessor::process_image, this);
	}

	void process_image(const sensor_msgs::Image img);
	void drive_robot(float lin_x, float ang_z);

private:
	unsigned find_col_of_first_white_pixel(const sensor_msgs::Image& img);
	enum class DriveDirection
	{
		kLeft,
		kForward,
		kRight,
		kStop
	};
	DriveDirection get_drive_direction(const unsigned col, const unsigned cols);
	struct LinearAngularVelocities
	{
		float linear_x;
		float angular_z;
	};
	LinearAngularVelocities convert_direction_to_velocities(const DriveDirection& drive_direction);
	ros::NodeHandle n_{};
	ros::ServiceClient command_robot_service_client_{};
	ros::Subscriber camera_images_subscriber_{};
};

unsigned ImageProcessor::find_col_of_first_white_pixel(const sensor_msgs::Image& img)
{
	constexpr auto white_pixel = 255;
	const auto rows = img.step;
	const auto cols = img.width;
	ROS_INFO_STREAM ("img.step = " << rows <<  ", img.width = " << cols);
	unsigned white_col{cols};
	auto i = 0U;
	while (i < rows * cols)
	{
		const auto pixel = img.data[i];
		const auto pixel_is_white = pixel == white_pixel;
		if (pixel_is_white)
		{
			ROS_INFO_STREAM("i = " << i);
			ROS_INFO_STREAM("cols = " << cols);
			white_col = i % cols;
			auto white_row = (i - white_col)/rows;
			ROS_INFO_STREAM ("col = " << white_col <<  ", row = " << white_row);
			ROS_INFO_STREAM("White ball was found");
			break;
		}
		++i;
	}
	return white_col;
}

ImageProcessor::DriveDirection ImageProcessor::get_drive_direction(const unsigned col, const unsigned cols)
{
	const auto image_left_col = cols / 3;
	const auto image_right_col = 2 * cols / 3;
	DriveDirection direction{DriveDirection::kForward};
	if (col == cols)
	{
		direction = DriveDirection::kStop;
	}
	else if (col < image_left_col)
	{
		direction = DriveDirection::kLeft;
	}
	else if (col > image_right_col)
	{
		direction = DriveDirection::kRight;
	}
	return direction;
}

ImageProcessor::LinearAngularVelocities ImageProcessor::convert_direction_to_velocities(const DriveDirection& drive_direction)
{
	LinearAngularVelocities velocities{};
	switch (drive_direction)
	{
		case DriveDirection::kLeft:
			velocities.linear_x = 0.0;
			velocities.angular_z = 0.2;
			break;
		case DriveDirection::kRight:
			velocities.linear_x = 0.0;
			velocities.angular_z = -0.2;
			break;
		case DriveDirection::kForward:
			velocities.linear_x = 0.1;
			velocities.angular_z = 0.0;
			break;
		default:
			velocities.linear_x = 0.0;
			velocities.angular_z = 0.0;
			break;						
	}
	return velocities;

}

void ImageProcessor::process_image(const sensor_msgs::Image img)
{
	ROS_INFO_STREAM("process_image_started ...");
	auto white_col = find_col_of_first_white_pixel(img);
	ROS_INFO_STREAM("col: " << white_col);
	auto drive_direction = get_drive_direction(white_col, img.width);
	auto velocities = convert_direction_to_velocities(drive_direction);
	drive_robot(velocities.linear_x, velocities.angular_z);
}
void ImageProcessor::drive_robot(float lin_x, float ang_z)
{
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;

	auto commands_executed_correctly = command_robot_service_client_.call(srv);
	if (!commands_executed_correctly)
		ROS_ERROR("Failed to call service drive_bot");

}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "process_image");
	ImageProcessor image_processor{};
	ros::spin();
	return 0;
}
