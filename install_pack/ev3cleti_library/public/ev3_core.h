#ifndef EV3_CORE_H_
#define EV3_CORE_H_

#ifndef __cplusplus
typedef enum bool_ {
	false = 0,
	true = 1
}bool;
#endif

#define INPUT_1  "in1"  //!< Sensor port 1
#define INPUT_2  "in2"  //!< Sensor port 2
#define INPUT_3  "in3"  //!< Sensor port 3
#define INPUT_4  "in4"  //!< Sensor port 4

#define OUTPUT_A "outA" //!< Motor port A
#define OUTPUT_B "outB" //!< Motor port B
#define OUTPUT_C "outC" //!< Motor port C
#define OUTPUT_D "outD" //!< Motor port D

#endif // EV3_CORE_H_
