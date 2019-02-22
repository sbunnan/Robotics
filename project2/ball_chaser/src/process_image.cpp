#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>


// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_robot(float lin_x, float ang_z)
{
    // TODO: Request a service and pass the velocities to it to drive the robot
    ball_chaser::DriveToTarget srv; 
    srv.request.linear_x = lin_x;
    srv.request.angular_z = ang_z;
   if (!client.call(srv))
        ROS_ERROR("Failed to call service command_robot");
   ros::Duration(1).sleep();

}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{

    int white_pixel = 255;
    bool left_flag = false;
    bool right_flag = false;
    bool middle_flag = false;
    int width =  img.step;
   // ROS_INFO("step : %d, height : %d, width: %d", width, img.height, img.width);
    int index = 0;
 
    // ros::Duration(10).sleep();
/*    for (int i = 0; i < img.height * img.step; ++i) {
        if (img.data[i]  == white_pixel) {
        	index = i%img.step;
        	if (index < width/3)
        	{
        		drive_robot(0, 0.2);
        		return;
        	}
        	else if ( (index >= width/3) && (index < 2*width/3))
        	{
            	drive_robot(0.5, 0);
            	return;
        	}
        	else if((index >= 2*width/3) && (index < width))
        	{
		   		drive_robot(0, -0.2);
		   		return;
        	}
        }
    }*/
    for (int i = 0; i < img.height; ++i)
    {
    	for (int j = 0; j < img.step; ++j)
    	{
	        if (img.data[i*img.step + j]  == white_pixel) {
	        	if (j < width/3)
	        	{
	        		drive_robot(0, 0.2);
	        		return;
	        	}
	        	else if ( (j >= width/3) && (j < 2*width/3))
	        	{
	            	drive_robot(0.3, 0);
	            	return;
	        	}
	        	else if((j >= 2*width/3) && (j < width))
	        	{
			   		drive_robot(0, -0.2);
			   		return;
	        	}
        	}
    	}
    	/* code */
    }

   drive_robot(0, 0);
   return;

    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera
}

int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
