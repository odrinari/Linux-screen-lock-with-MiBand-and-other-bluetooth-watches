# Linux screen lock with MiBand

**Hi!**

This application uses bluetooth and mac address of your MiBand or other wearable watch to lock or unlock screen of the Linux operating system when you leave or return to the computer.
You can use a small guide on YouTube to install this package!

![BtUnlock_-example-of-work-_720p_-_online-video-cutter com_-_2_](https://user-images.githubusercontent.com/40739802/227795137-139ca856-f9eb-4744-92c7-e26ef21baf79.gif)


## Youtube guide
Link to a youtube video with an example of installation and operation ```https://youtu.be/CxZC94-KWyc```

## Text guide

 * Enter the MAC address of your BLE device
 
 * Save and run the ```btunlock -d``` process via the console (**this is a daemon process, if desired, it will work on the background constantly**)
 
 * Experiment with incoming values at certain distances between the device and the computer
 
 * Tick the desired operating modes
 
 * Set the thresholds and save the configuration
 
 * If you have entered new values restart the process ```btunlock -d```
 
 * the process starts automatically after logout and reboot with saved settings
 

<hr>

You can find it in the Mi Fit app. To do this, go to the MiFit app, then to ```Profiles > Mi Bank > Bluetooth Address```
