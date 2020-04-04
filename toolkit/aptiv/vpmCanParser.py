#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2019-08-23 14:44:51
####################################################

class IntBitArry(object):
    def __init__(self, integer):
        bits = bin(integer)
        bits = bits.replace('b', '0')
        if len(bits) < 8:
            bits = '0' * (8-len(bits)) + bits
        self.integer = bits
    def __len__(self):
        return len(self.integer)
    def __getitem__(self, item):
        bits = self.integer[::-1][item][::-1]
        # print("%s[%s] : %s" % (int(self.integer,2),item,bits))
        return bits
    def __setitem__(self, key, value):
        self.integer = self.integer[::-1]
        try:
            self.integer[key] = value
        except Exception as reason:
            print("error: %s" % reason)
            pass
        finally:
            self.integer = self.integer[::-1]
    def __str__(self):
        return str(self.integer)

def Infotainment_Operation_LS_proc(data):
    """
    byte[0][1]     Valet Mode Active
    byte[0][0]     Infotainment Operation Allowed
    byte[0][2]     Front 360 Camera Switch Active
    byte[0][3]     Camera Video ICS Display Active
    byte[0][4]     Left Rear Seat Status Display Active
    byte[0][5]     Right Rear Seat Status Display Active
    byte[1][::3]   Rear Pedestrian Detection Customization Setting Request
    byte[0][6]     Triggered Video Recorder Switch Active
    byte[2][::2]   Integrated Center Stack Touch Status : Touch Proximity
    byte[3::5][::] Integrated Center Stack Touch Status : X1 Touch Coordinate
    byte[5:7][::]  Integrated Center Stack Touch Status : Y1 Touch Coordinate
    byte[2][5::]   Connected Vehicle Braking Alerts Customization Setting Request
    byte[1][3::6]  Intersection Stop Alert Customization Setting Request
    byte[2][2::4]  Traffic and Roadside Information Customization Setting Request
    """
    Info = {
    'Valet_Mode_Active': 'Valet Mode Active',
    'Infotainment_Operation_Allowed': 'Infotainment Operation Allowed',
    'Front_360_Camera_Switch_Active': 'Front 360 Camera Switch Active',
    'Camera_Video_ICS_Display_Active': 'Camera Video ICS Display Active',
    'Left_Rear_Seat_Status_Display_Active': 'Left Rear Seat Status Display Active',
    'Right_Rear_Seat_Status_Display_Active': 'Right Rear Seat Status Display Active',
    'Rear_Pedestrian_Detection_Customization_Setting_Request': 'Rear Pedestrian Detection Customization Setting Request',
    'Triggered_Video_Recorder_Switch_Active': 'Triggered Video Recorder Switch Active',
    'Integrated_Center_Stack_Touch_Status___Touch_Proximity': 'Integrated Center Stack Touch Status : Touch Proximity',
    'Integrated_Center_Stack_Touch_Status___X1_Touch_Coordinate': 'Integrated Center Stack Touch Status : X1 Touch Coordinate',
    'Integrated_Center_Stack_Touch_Status___Y1_Touch_Coordinate': 'Integrated Center Stack Touch Status : Y1 Touch Coordinate',
    'Connected_Vehicle_Braking_Alerts_Customization_Setting_Request': 'Connected Vehicle Braking Alerts Customization Setting Request',
    'Intersection_Stop_Alert_Customization_Setting_Request': 'Intersection Stop Alert Customization Setting Request',
    'Traffic_and_Roadside_Information_Customization_Setting_Request': 'Traffic and Roadside Information Customization Setting Request',
    }

    can_id = 0x1036A080
    can_data = "9 0   2   7F  DD  EB  0B"
    can_data = data
    print("Infotainment_Operation_LS_proc: ################## %s ##################" % can_data)
    can_data = can_data.strip()
    if not can_data:
        print("invalid can data")
        return
    byte = [ int(bt, 16) for bt in can_data.split() ]

    byte0 = IntBitArry(byte[0])
    byte1 = IntBitArry(byte[1])
    byte2 = IntBitArry(byte[2])
    #byte[0][1]     
    Valet_Mode_Active = int(byte0[1], 2)
    #byte[0][0]     
    Infotainment_Operation_Allowed = int(byte0[0], 2)
    #byte[0][2]     
    Front_360_Camera_Switch_Active = int(byte0[2], 2)
    #byte[0][3]     
    Camera_Video_ICS_Display_Active = int(byte0[3], 2)
    #byte[0][4]     
    Left_Rear_Seat_Status_Display_Active = int(byte0[4], 2)
    #byte[0][5]     
    Right_Rear_Seat_Status_Display_Active = int(byte0[5], 2)
    #byte[1][::3]   
    Rear_Pedestrian_Detection_Customization_Setting_Request = int(byte1[::3], 2)
    #byte[0][6]     
    Triggered_Video_Recorder_Switch_Active = int(byte0[6], 2)
    #byte[2][::2]   
    Integrated_Center_Stack_Touch_Status___Touch_Proximity = int(byte2[::2], 2)
    #byte[3::5][::] 
    Integrated_Center_Stack_Touch_Status___X1_Touch_Coordinate = hex((byte[3]<<8) + byte[4])
    #byte[5:7][::]  
    Integrated_Center_Stack_Touch_Status___Y1_Touch_Coordinate = hex((byte[5]<<8) + byte[6])
    #byte[2][5::]   
    Connected_Vehicle_Braking_Alerts_Customization_Setting_Request = int(byte2[5::], 2)
    #byte[1][3::6]  
    Intersection_Stop_Alert_Customization_Setting_Request = int(byte1[3::6], 2)
    #byte[2][2::4]  
    Traffic_and_Roadside_Information_Customization_Setting_Request = int(byte2[2::4], 2)
    for key in Info:
        print("%s: %s" % (Info[key], eval(key)))
    print("Infotainment_Operation_LS_proc: ################## end ##################")

def Front_360_Camera_On_LS_proc(data):
    """
    byte[0][0]     Display Front 360 Camera On
    byte[2][0]     Display Smart Tow Video On
    byte[0][1]     Rear Pedestrian Detection Customization Setting Available
    byte[0][2::5]  Rear Pedestrian Detection Customization Current Setting Value
    byte[0][5::7]  Rear Pedestrian Detection Haptic Seat Vibration Request Sequence Number
    byte[1][::6]   Rear Pedestrian Detection Haptic Seat Vibration Request
    byte[0][7]     Triggered Video Recorder Feature Present
    byte[1][7]     360 Degree Video Feature Present
    byte[1][6]     Display Triggered Video On
    byte[2][7]     Rear Pedestrian Detection Customization Available : Off Available
    byte[2][6]     Rear Pedestrian Detection Customization Available : On Available
    byte[2][5]     Rear Pedestrian Detection Customization Available : Alert Available
    byte[2][4]     Rear Pedestrian Detection Customization Available : Alert and Brake Available
    byte[2][3]     Rear Pedestrian Detection Customization Available : Reserved1 Available
    byte[2][2]     Rear Pedestrian Detection Customization Available : Reserved2 Available
    byte[2][1]     Rear Pedestrian Detection Customization Available : Reserved3 Available
    """
    Info = {
    'Display_Front_360_Camera_On': 'Display Front 360 Camera On',
    'Display_Smart_Tow_Video_On': 'Display Smart Tow Video On',
    'Rear_Pedestrian_Detection_Customization_Setting_Available': 'Rear Pedestrian Detection Customization Setting Available',
    'Rear_Pedestrian_Detection_Customization_Current_Setting_Value': 'Rear Pedestrian Detection Customization Current Setting Value',
    'Rear_Pedestrian_Detection_Haptic_Seat_Vibration_Request_Sequence_Number': 'Rear Pedestrian Detection Haptic Seat Vibration Request Sequence Number',
    'Rear_Pedestrian_Detection_Haptic_Seat_Vibration_Request': 'Rear Pedestrian Detection Haptic Seat Vibration Request',
    'Triggered_Video_Recorder_Feature_Present': 'Triggered Video Recorder Feature Present',
    'a360_Degree_Video_Feature_Present': '360 Degree Video Feature Present',
    'Display_Triggered_Video_On': 'Display Triggered Video On',
    'Rear_Pedestrian_Detection_Customization_Available___Off_Available': 'Rear Pedestrian Detection Customization Available : Off Available',
    'Rear_Pedestrian_Detection_Customization_Available___On_Available': 'Rear Pedestrian Detection Customization Available : On Available',
    'Rear_Pedestrian_Detection_Customization_Available___Alert_Available': 'Rear Pedestrian Detection Customization Available : Alert Available',
    'Rear_Pedestrian_Detection_Customization_Available___Alert_and_Brake_Available': 'Rear Pedestrian Detection Customization Available : Alert and Brake Available',
    'Rear_Pedestrian_Detection_Customization_Available___Reserved1_Available': 'Rear Pedestrian Detection Customization Available : Reserved1 Available',
    'Rear_Pedestrian_Detection_Customization_Available___Reserved2_Available': 'Rear Pedestrian Detection Customization Available : Reserved2 Available',
    'Rear_Pedestrian_Detection_Customization_Available___Reserved3_Available': 'Rear Pedestrian Detection Customization Available : Reserved3 Available',
    }

    can_id = 0x1050C06D
    can_data = "8E  80  A1  0   0   0   0"
    can_data = data
    print("Front_360_Camera_On_LS_proc: ################## %s ##################" % can_data)
    can_data = can_data.strip()
    if not can_data:
        print("invalid can data")
        return
    byte = [ int(bt, 16) for bt in can_data.split() ]

    byte0 = IntBitArry(byte[0])
    byte1 = IntBitArry(byte[1])
    byte2 = IntBitArry(byte[2])

    #byte[0][0]     
    Display_Front_360_Camera_On = int(byte0[0], 2)
    #byte[2][0]     
    Display_Smart_Tow_Video_On = int(byte2[0], 2)
    #byte[0][1]     
    Rear_Pedestrian_Detection_Customization_Setting_Available = int(byte0[1], 2)
    #byte[0][2::5]  
    Rear_Pedestrian_Detection_Customization_Current_Setting_Value = int(byte0[2::5], 2)
    #byte[0][5::7]  
    Rear_Pedestrian_Detection_Haptic_Seat_Vibration_Request_Sequence_Number = int(byte0[5::7], 2)
    #byte[1][::6]   
    Rear_Pedestrian_Detection_Haptic_Seat_Vibration_Request = int(byte1[::6], 2)
    #byte[0][7]     
    Triggered_Video_Recorder_Feature_Present = int(byte0[7], 2)
    #byte[1][7]     
    a360_Degree_Video_Feature_Present = int(byte1[7], 2)
    #byte[1][6]     
    Display_Triggered_Video_On = int(byte1[6], 2)
    #byte[2][7]     
    Rear_Pedestrian_Detection_Customization_Available___Off_Available = int(byte2[7], 2)
    #byte[2][6]     
    Rear_Pedestrian_Detection_Customization_Available___On_Available = int(byte2[6], 2)
    #byte[2][5]     
    Rear_Pedestrian_Detection_Customization_Available___Alert_Available = int(byte2[5], 2)
    #byte[2][4]     
    Rear_Pedestrian_Detection_Customization_Available___Alert_and_Brake_Available = int(byte2[4], 2)
    #byte[2][3]     
    Rear_Pedestrian_Detection_Customization_Available___Reserved1_Available = int(byte2[3], 2)
    #byte[2][2]     
    Rear_Pedestrian_Detection_Customization_Available___Reserved2_Available = int(byte2[2], 2)
    #byte[2][1]     
    Rear_Pedestrian_Detection_Customization_Available___Reserved3_Available = int(byte2[1], 2)
    for key in Info:
        print("%s: %s" % (Info[key], eval(key)))
    print("Front_360_Camera_On_LS_proc: ################## end ##################")

def test():
    can_data = "9 0   2   7F  DD  EB  0B"
    Infotainment_Operation_LS_proc(can_data)
    can_data = "8E  80  A1  0   0   0   0"
    Front_360_Camera_On_LS_proc(can_data)

if __name__ == "__main__":
    test()

