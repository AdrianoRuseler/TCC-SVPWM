################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
utility\ Src\ Files/DLOG_4CH_F.obj: ../utility\ Src\ Files/DLOG_4CH_F.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/c2000/bin/cl2000" -v28 -ml -mt --float_support=fpu32 --cla_support=cla1 --tmu_support=tmu0 --vcu_support=vcu2 --opt_for_speed=5 --fp_mode=relaxed --include_path="C:/ti/c2000/include" --include_path="C:/Users/ESPM/Documents/workspace_cc7/blinky_cpu01/utility Src Files" --include_path="C:/ti/controlSUITE/libs/app_libs/motor_control/math_blocks/v4.3" --include_path="C:/ti/controlSUITE/libs/math/IQmath/v160/include" --include_path="C:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_headers/include" --include_path="C:/ti/controlSUITE/device_support/F2837xS/v210/F2837xS_common/include" -g --define=CPU1 --display_error_number --diag_suppress=10063 --diag_warning=225 --preproc_with_compile --preproc_dependency="utility Src Files/DLOG_4CH_F.d_raw" --obj_directory="utility Src Files" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


