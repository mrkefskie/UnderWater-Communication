################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
protocol/decode.obj: ../protocol/decode.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/arm_15.12.2.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="E:/Data/TI/Frequency detect + protocol" --include_path="C:/ti/msp430/MSPWare_3_30_00_18/driverlib/driverlib/MSP432P4xx/" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.2.LTS/include/" --include_path="C:/ti/ccsv6/tools/compiler/arm_15.12.2.LTS/include" --advice:power=all -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_warning=225 --display_error_number --diag_wrap=off --preproc_with_compile --preproc_dependency="protocol/decode.d" --obj_directory="protocol" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


