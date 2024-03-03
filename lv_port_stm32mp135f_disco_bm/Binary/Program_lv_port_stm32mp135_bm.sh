#!/bin/bash
#--------

# Check if STM32_Programmer_CLI is available in the $PATH
cube_path=`which STM32_Programmer_CLI`
if [ ! -x "${cube_path}" ]; then
  error "STM32_Programmer_CLI not found in the PATH"
  echo "Please install STM32CubeProgrammer : "
  echo "https://wiki.st.com/stm32mpu/wiki/STM32CubeProgrammer#Installing_the_STM32CubeProgrammer_tool"
  \popd >/dev/null 2>&1
  exit 1
fi

# Use date to set unique tmp file name
tmp_version=`date +%Y-%m-%d_%H-%M`

STM32_Programmer_CLI -l usb > /tmp/stm32_programmer_result-${tmp_version}

device_not_found=$(cat /tmp/stm32_programmer_result-${tmp_version} | grep "No STM32 device in DFU mode connected")

if [[ -n "${device_not_found}" ]] ; then
  echo "${device_not_found}"
  exit 1
fi

usb_dfu_list=()
while read -r line; do
  usb_dfu_list+=("$line")
done < <(cat /tmp/stm32_programmer_result-${tmp_version} | grep "Device Index" | awk -F ': ' '{ print $2 }')

user_device_serial_list=()
while read -r line; do
  user_device_serial_list+=("$line")
done < <(cat /tmp/stm32_programmer_result-${tmp_version} | grep "Serial number" | awk -F ': ' '{ print $2 }')
\rm -f /tmp/stm32_programmer_result-${tmp_version}

if [ "${#usb_dfu_list[@]}" -eq 0 ]; then
  error "No device detected, check that it's connected and configured in DFU"
  \popd >/dev/null 2>&1
  exit 1
fi

if [ "${#user_device_serial_list[@]}" -eq 0 ]; then
  error "No device detected, check that it's connected and configured in DFU"
  \popd >/dev/null 2>&1
  exit 1
fi

# Check how much devices are available
num_devices=${#user_device_serial_list[@]}

# Test is USER_DEVICE_SERIAL is already defined and if it correspond to one of the existing devices plugger to PC.
if [[ -n "${USER_DEVICE_SERIAL+1}" ]]; then
  for ((i=0; i<num_devices; i++)); do
    if [ "$USER_DEVICE_SERIAL" == "${user_device_serial_list[i]}" ]; then
      chosen_device=${i}
    fi
  done

  if [ ${chosen_device} == -1 ]; then
    error "The defined serial number from your environment (USER_DEVICE_SERIAL) was not found among the available devices"
    \popd >/dev/null 2>&1
    exit 1
  fi
  echo "Flashing the device $USER_DEVICE_SERIAL"
# If there is no USER_DEVICE_SERIAL number defined
else
  # If there is only one device, it choose it by default
  if [ ${num_devices} -eq 1 ]; then
    chosen_device=0
    echo "Flashing the device ${user_device_serial_list[0]}"
  # Otherwise, it prints all the possibilities with an associated number so
  # that the user can choose the device he wants to flash.
  else
    for j in {0..NUMBER_OF_TRY}; do
      echo "Available devices:"
      for ((i=0; i<num_devices; i++)); do
        echo "$i. ${user_device_serial_list[i]}"
      done

      read -p "Enter the id of the device you want to use: " chosen_device

      if ! [ ${chosen_device} -ge 0 ] || ! [ ${chosen_device} -le ${num_devices} ] || ! [[ $chosen_device =~ ^[0-9]+$ ]]; then
        if [ ${j} == NUMBER_OF_TRY ]; then
          error "Invalid selection"
          \popd >/dev/null 2>&1
          exit 1
        fi
        error "Invalid value, please choose a number between 0 and ${num_devices}"
        empty_line
      else
        echo "Flashing the device ${user_device_serial_list[chosen_device]}"
        break
      fi
    done
  fi
fi

usb_dfu=${usb_dfu_list[chosen_device]}

#Get tsv list for the current memory type configuration
tsv_list=$(find flashlayout -name "*.tsv" | awk -F"/" '{print $NF}')
options=($tsv_list)

if [[ ${#options[@]} -eq 1 ]]; then
  echo "Layout config ${options[0]} selected"
  selected_tsv=${options[0]}
else
  PS3='Which layout do you want to flash ? '
  select opt in "${options[@]}"
  do
    selected_tsv=$opt
    break
  done
fi

\STM32_Programmer_CLI -c port=${usb_dfu} -w flashlayout/${selected_tsv}
