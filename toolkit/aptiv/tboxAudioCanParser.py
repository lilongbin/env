#! /usr/bin/env python
# coding=utf-8
####################################################
# Author      : longbin
# Created date: 2018-09-26 14:04:18
####################################################
import os
import sys

segSeparator = "\t"
sys_version = sys.version_info[0]
logfilename = ""

def logwrite(line):
    if not logfilename.strip():
        # print("logfilename is empty")
        return
    # if not os.path.isfile(logfilename):
    #     pass
    with open(logfilename, 'a+') as fd:
        fd.write(line + os.linesep)

def logmsg(*args):
    __print_func="print(info)," if sys_version == 2 else "print(info, end='')"
    for info in args:
        exec(__print_func)
        logwrite(info)
    print("")

def AudioMasterArbitrationCommand(msg):
    # [ICI] --> [TBOx]
    # 4 4 3 5
    """
    Audio Master Source Type 5
    0x1C = Phone,
    0x18 = SDARS,
    0x14 = DAB,
    0x10 = Remote CDX,
    0x0C = Remote DVD,
    0X08 = Auxiliary Device,
    0x00 = All for Supervising,
    0x01 = Television Tuner

    Audio Master Channel Type 3
    0x0 = No Channel,
    0x1 = Mono,
    0x2 = Stereo,
    0x3 = Mixed,
    0x4 = Mono Emergency,
    0x5-0x7 Reserved

    Audio Master Arbitration Command 4
    0x0 = No Action,
    0x1 = Do not Connect,
    0x2 = Connect,
    0x3 = Disconnect,
    0x4 = Connection Complete

    Logical AV Channel 4
    0x3 - Asynchronous Message,
    0x4 - Incoming Call,
    0x5 - Phone,
    0x6 - Synchronous Message,
    0x7 - Speech Recognition,
    0x8 - Emergency Phone Call,
    0x9 - Short MIX Alert Message,
    0xC - OnStar Initiated Call, 
    """
    # logmsg(msg)
    cmdstring = ""
    msg = "".join(msg).strip()
    if (len(msg) < 4):
        logmsg("msg len should not less than 4")
        return "invalid can msg"

    # AudioMasterSourceType 5
    AudioMasterSourceType = ((int(msg[2:4], 16) >> 0)& 0x1F)
    AudioMasterSourceTypeDict = {
            0x1C : "Phone",
            0x18 : "SDARS",
            0x14 : "DAB",
            0x10 : "Remote_CDX",
            0x0C : "Remote_DVD",
            0x08 : "Auxiliary_Device",
            0x00 : "All_for_Supervising",
            0x01 : "Television_Tuner",
            }
    if AudioMasterSourceType in AudioMasterSourceTypeDict:
        cmdstring = "[SrcType:" + AudioMasterSourceTypeDict[AudioMasterSourceType] + "] " + cmdstring
    else:
        cmdstring = "[SrcType:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    # AudioMasterChannelType 3
    AudioMasterChannelType = ((int(msg[2:4], 16) >> 5) & (0x8 - 1))
    AudioMasterChannelTypeDict = {
            0x0 : "No_Channel",
            0x1 : "Mono",
            0x2 : "Stereo",
            0x3 : "Mixed",
            0x4 : "Mono_Emergency",
            0x5 : "Reserved",
            0x6 : "Reserved",
            0x7 : "Reserved",
            }
    if AudioMasterChannelType in AudioMasterChannelTypeDict:
        cmdstring = "[ChnType:" + AudioMasterChannelTypeDict[AudioMasterChannelType] + "] " + cmdstring
    else:
        cmdstring = "[ChnType:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    # AudioMasterArbitrationCommand 4
    AudioMasterArbitrationCommand = int(msg[1], 16)
    AudioMasterArbitrationCommandDict = {
            0x0 : "No_Action",
            0x1 : "Do_not_Connect",
            0x2 : "Connect",
            0x3 : "Disconnect",
            0x4 : "Connection_Complete",
            }
    if AudioMasterArbitrationCommand in AudioMasterArbitrationCommandDict:
        cmdstring = "[ArbCmd:" + AudioMasterArbitrationCommandDict[AudioMasterArbitrationCommand] + "] " + cmdstring
    else:
        cmdstring = "[ArbCmd:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    # LogicalAVChannel 4
    LogicalAVChannel = int(msg[0], 16)
    LogicalAVChannelDict = {
            0x3 : "Asynchronous_Message",
            0x4 : "Incoming_Call",
            0x5 : "Phone",
            0x6 : "Synchronous_Message",
            0x7 : "Speech_Recognition",
            0x8 : "Emergency_Phone_Call",
            0x9 : "Short_MIX_Alert_Message",
            0xC : "OnStar_Initiated_Call", 
            }
    if LogicalAVChannel in LogicalAVChannelDict:
        cmdstring = "[LAVChn:" + LogicalAVChannelDict[LogicalAVChannel] + "] " + cmdstring
    else:
        cmdstring = "[LAVChn:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring
    cmdstring = "[ICI] --> [TBox] " + msg + cmdstring
    return cmdstring

def AudioSourceStatus(msg):
    # [ICI] <-- [TBOx]
    # 4 5 3 4
    """
    Audio Source Type   5
    0x1C = Phone,
    0x18 = SDARS,
    0x14 = DAB,
    0x10 = Remote CDX,
    0x0C = Remote DVD,
    0X08 = Auxiliary Device,
    0x00 = All for Supervising,
    0x01 = Television Tuner,
    Always set to 0x1C by Telematics/OnStar Module

    Audio Source Channel Type   3
    0x0=No Action,
	0x1=Mono,
	0x2=Stereo,
	0x3=Mixed,
	0x4=Mono Emergency,
	0x5-0x7 Reserved,

    Audio Source Status Code    4
    0x0=Device Present,
	0x1=Audio Available,
	0x2=Audio Not Available,
	0x3=Reserved,
	0x4=Producing Silence,
	0x5=Producing Audio,

    Logical AV Channel  4
    0x3 - Asynchronous Message,
    0x4 - Incoming Call,
    0x5 - Phone,
    0x6 - Synchronous Message,
    0x7 - Speech Recognition,
    0x8 - Emergency Phone Call,
    0x9 - Short MIX Alert Message,
    0xC - OnStar Initiated Call, 
    """
    # logmsg(msg)
    cmdstring = ""
    msg = "".join(msg).strip()
    if (len(msg) < 4):
        logmsg("msg len should not less than 4")
        return "invalid can msg"

    AudioSourceStatusCode = int(msg[3], 16)
    AudioSourceStatusCodeDict = {
            0x0 : "Device_Present",
	        0x1 : "Audio_Available",
	        0x2 : "Audio_Not_Available",
	        0x3 : "Reserved",
	        0x4 : "Producing_Silence",
	        0x5 : "Producing_Audio",
            }
    if AudioSourceStatusCode in AudioSourceStatusCodeDict:
        cmdstring = "[SrcStat:" + AudioSourceStatusCodeDict[AudioSourceStatusCode]+ "] " + cmdstring
    else:
        cmdstring = "[SrcStat:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    AudioSourceChannelType = ((int(msg[1:3], 16) >> 0) & (0x8 - 1))
    AudioSourceChannelTypeDict = {
            0x0 : "No_Action",
	        0x1 : "Mono",
	        0x2 : "Stereo",
	        0x3 : "Mixed",
	        0x4 : "Mono_Emergency",
	        0x5 : "Reserved",
            0x6 : "Reserved",
            0x7 : "Reserved",
            }
    if AudioSourceChannelType in AudioSourceChannelTypeDict:
        cmdstring = "[ChnType:" + AudioSourceChannelTypeDict[AudioSourceChannelType] + "] " + cmdstring
    else:
        cmdstring = "[ChnType:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    AudioSourceType = ((int(msg[1:3], 16) >> 3)& 0x1F)
    # Always set to 0x1C by Telematics/OnStar Module
    AudioSourceTypeDict = {
            0x1C : "Phone",
            0x18 : "SDARS",
            0x14 : "DAB",
            0x10 : "Remote_CDX",
            0x0C : "Remote_DVD",
            0X08 : "Auxiliary_Device",
            0x00 : "All_for_Supervising",
            0x01 : "Television_Tuner",
            }
    if AudioSourceType in AudioSourceTypeDict:
        cmdstring = "[SrcType:" + AudioSourceTypeDict[AudioSourceType] + "] " + cmdstring
    else:
        cmdstring = "[SrcType:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring

    LogicalAVChannel = int(msg[0], 16)
    LogicalAVChannelDict = {
            0x3 : "Asynchronous Message",
            0x4 : "Incoming_Call",
            0x5 : "Phone",
            0x6 : "Synchronous_Message",
            0x7 : "Speech_Recognition",
            0x8 : "Emergency_Phone_Call",
            0x9 : "Short_MIX_Alert_Message",
            0xC : "OnStar_Initiated_Call", 
            }
    if LogicalAVChannel in LogicalAVChannelDict:
        cmdstring = "[LAVChn:" + LogicalAVChannelDict[LogicalAVChannel] + "] " + cmdstring
    else:
        cmdstring = "[LAVChn:unknown] " + cmdstring
    cmdstring = segSeparator + cmdstring
    cmdstring = "[ICI] <-- [TBox] " + msg + cmdstring
    return cmdstring

def msg_dispatch(CanId, CanMsg):
    if CanId in [0x367, 0x368, 0x106D0080]:
        # radio --> T-Box
        # logmsg("%#X %s" % (CanId, CanMsg))
        return AudioMasterArbitrationCommand(CanMsg)
    elif CanId in [0x370, 0x371, 0x106E0097]:
        # radio <-- T-Box
        # logmsg("%#X %s" % (CanId, CanMsg))
        return AudioSourceStatus(CanMsg)

def parse_audio_can(msg):
    if len(msg.split()) < 9:
        return None
    msg = msg.strip().split()
    MsgNo = msg[0]
    Port = msg[1]
    TS = msg[2]
    Bus = msg[3]
    TxRx = msg[4]
    # logmsg(" ".join(msg))
    try:
        int(MsgNo)
    except Exception as e:
        # logmsg(msg, e)
        return None
    if Bus not in ["CAN1", "CAN2"]:
        return None
    CanMsgId = "".join(msg[5:7])
    if len(CanMsgId) < 4:
        return None
    CanId = int(CanMsgId, 16)
    CanData = "".join(msg[7:])
    # logmsg(CanData)
    # logmsg("%s %d" % (MsgNo, CanId))
    CanMsg = CanData
    # logmsg("%#X: %s" % (CanId, CanMsg))
    return msg_dispatch(CanId, CanMsg)

def parser(filename):
    msg = ""
    type = "txt"
    if filename.endswith(".csv") or filename.endswith(".csv.txt"):
        type = "csv"
    logmsg("type: %s" % (type))
    if type == "csv":
    # 164, 6.606196105, 0.006281614, 67371012, F, F, SW CAN, $106D0080, SW CAN, 1, 106D0080, F, T, 53, 3C
        logmsg("Please make sure that no empty cell in every line")
        with open(filename, "r") as rfd:
            for line in rfd:
                line = line.strip().split(",")
                # logmsg(line)
                if len(line) <= 13:
                    # 13 cells at least
                    continue
                if len(line[9]) < 3:
                    # can id
                    continue
                try:
                    CanId = "%08X" % int(line[9], 16)
                except:
                    continue
                if not len(line[12].strip()): line[12] = "0"
                if not len(line[13].strip()): line[13] = "0"
                msgValueB1 = "%02X" % int(line[12].strip(), 16)
                msgValueB2 = "%02X" % int(line[13].strip(), 16)
                line = [line[0],"CSV","0","CAN2",line[8],CanId[:2],CanId[2:], msgValueB1,msgValueB2]
                line = segSeparator.join(line)
                # logmsg(" ".join(line))
                msg = parse_audio_can(line)
                if msg:
                    msg = line + " >>: " + segSeparator + msg
                    logmsg(msg)
                else:
                    pass
                    # msg = line
                    # logmsg(msg)
    else:
    # 5041  SZ2000  0   CAN2    Rx  03 68 C3 3C
        with open(filename, "r") as rfd:
            for line in rfd:
                line = line.strip()
                msg = parse_audio_can(line)
                if msg:
                    msg = line + " >>: " + segSeparator + msg
                    logmsg(msg)
                else:
                    pass
                    # msg = line
                    # logmsg(msg)

def usage():
    info = """use this command to parse tbox/onstar audio can msg
    Usage:
        python tboxAudioCanParser.py <can.log>
    """
    logmsg(info)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        usage()
        sys.exit()
    if sys.argv[1].lower() in ["-h", "--help"]:
        usage()
        sys.exit()
    canfile = sys.argv[1]
    logfilename = canfile + ".log"
    if os.path.isfile(logfilename): os.rename(logfilename, canfile + ".bk.log")
    parser(canfile)

