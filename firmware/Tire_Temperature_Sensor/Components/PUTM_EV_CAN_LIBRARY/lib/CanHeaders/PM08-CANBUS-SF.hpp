//Generated on Tue Jun 28 18:36:51 2022
#ifndef SF
#define SF

#include <cstdint>

namespace PUTM_CAN {

enum struct SmartFuseState : uint8_t
{
        Ok,
        ResetState,
        SPIError,
        /*
         * over temperature event +
         * power limitation event +
         * VDS mask
         */
        OTPLVDS,
        TempFail,
        LatchOff,
        /*
         * open load event  +
         * shorted to ground
         */
        OLOFF,
        /*
         * device enters FailSafe on start up or when watch dog is
         * failed to be toggled within every 30 ms to 70 ms
         */
        FailSafe,
        NotResponding,
};
enum struct ChannelState : uint8_t
{
        Ok,
        UnderCurrent,
        OverCurrent,
        LatchOff,
        /*
         *  Output stuck to VCC/openload off state status.
         */
        STKFLTR,
        /*
         * This bit is ‘1’ if VDS is high at turn-off,
         * indicative of a potential overload condition
         */
        VDSFS,
        /*
         * Channel feedback status. Combination of Power limitation, OT,
         * OVERLOAD detection (VDS at turn-off). This bit is latched
         * during OFF-state of the channel in order to allow asynchronous
         * diagnostic and it is automatically cleared when the PL/OT/VDS
         * junction temperature falls below the thermal reset temperature
         * of OT detection, TRS.
         */
        CHFBSR,
        Error
};

enum struct SF_states: uint8_t {
	Ok,
	Warning_1,
	Warning_2,
	Error,
};

struct __attribute__ ((packed)) SF_main{
	SF_states device_state; 
	SmartFuseState fuse_0_state; // status returned by smart fuse
	SmartFuseState fuse_1_state; // status returned by smart fuse
	SmartFuseState fuse_2_state; // status returned by smart fuse
	SmartFuseState fuse_3_state; // status returned by smart fuse
};

struct __attribute__ ((packed)) SF_PassiveElements{
	ChannelState break_light; 
	ChannelState fan_mono; 
	ChannelState fan_l; 
	ChannelState fan_r; 
	ChannelState wheel_speed_1; // idk which is left and which is right
	ChannelState wheel_speed_2; // idk which is left and which is right or front or whatever
	ChannelState water_potentiometer; // they are together
	ChannelState tsal_assi; // supply for leds ex.
};

struct __attribute__ ((packed)) SF_LegendaryDVAndSupply{
	ChannelState lidar; 
	ChannelState box_dv; 
	ChannelState jetson; 
	ChannelState odrive; 
	ChannelState tsal; 
	ChannelState bspd_esb; 
	ChannelState spare_1; 
	ChannelState spare_2; 
};

struct __attribute__ ((packed)) SF_Supply{
	ChannelState inverter; 
	ChannelState front_box; 
	ChannelState dash; 
	ChannelState laptimer; 
	ChannelState bat_hv; 
	ChannelState diagport; 
	ChannelState pomp; 
	ChannelState motec; 
};

struct __attribute__ ((packed)) SF_safety{
	bool  firewall; 
	bool  hvd; 
	bool  inverter; 
	bool  dv; 
	bool  tsms; // 
};


const uint16_t SF_MAIN_CAN_ID = 0x41;
const uint8_t SF_MAIN_CAN_DLC = sizeof(SF_main);
const uint8_t SF_MAIN_FREQUENCY = 100;
const uint16_t SF_PASSIVEELEMENTS_CAN_ID = 0x46;
const uint8_t SF_PASSIVEELEMENTS_CAN_DLC = sizeof(SF_PassiveElements);
const uint8_t SF_PASSIVEELEMENTS_FREQUENCY = 10;
const uint16_t SF_LEGENDARYDVANDSUPPLY_CAN_ID = 0x4b;
const uint8_t SF_LEGENDARYDVANDSUPPLY_CAN_DLC = sizeof(SF_LegendaryDVAndSupply);
const uint8_t SF_LEGENDARYDVANDSUPPLY_FREQUENCY = 10;
const uint16_t SF_SUPPLY_CAN_ID = 0x50;
const uint8_t SF_SUPPLY_CAN_DLC = sizeof(SF_Supply);
const uint8_t SF_SUPPLY_FREQUENCY = 10;
const uint16_t SF_SAFETY_CAN_ID = 0x3d;
const uint8_t SF_SAFETY_CAN_DLC = sizeof(SF_safety);
const uint8_t SF_SAFETY_FREQUENCY = 1;

const CAN_TxHeaderTypeDef can_tx_header_SF_MAIN{
SF_MAIN_CAN_ID, 0xFFF, CAN_ID_STD, CAN_RTR_DATA, SF_MAIN_CAN_DLC, DISABLE};

const CAN_TxHeaderTypeDef can_tx_header_SF_PASSIVEELEMENTS{
SF_PASSIVEELEMENTS_CAN_ID, 0xFFF, CAN_ID_STD, CAN_RTR_DATA, SF_PASSIVEELEMENTS_CAN_DLC, DISABLE};

const CAN_TxHeaderTypeDef can_tx_header_SF_LEGENDARYDVANDSUPPLY{
SF_LEGENDARYDVANDSUPPLY_CAN_ID, 0xFFF, CAN_ID_STD, CAN_RTR_DATA, SF_LEGENDARYDVANDSUPPLY_CAN_DLC, DISABLE};

const CAN_TxHeaderTypeDef can_tx_header_SF_SUPPLY{
SF_SUPPLY_CAN_ID, 0xFFF, CAN_ID_STD, CAN_RTR_DATA, SF_SUPPLY_CAN_DLC, DISABLE};

const CAN_TxHeaderTypeDef can_tx_header_SF_SAFETY{
SF_SAFETY_CAN_ID, 0xFFF, CAN_ID_STD, CAN_RTR_DATA, SF_SAFETY_CAN_DLC, DISABLE};

} //namespace can

#endif

