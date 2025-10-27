{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{

  buildInputs = with pkgs; [
      doxygen
	  arduino-cli
	  arduino-ide
	  usbutils
	  udev
	  glibc_multi
	  #newlib
	  ];

  #export CPLUS_INCLUDE_PATH=~/.arduino15/packages/rp2040/hardware/rp2040/5.2.1/cores/rp2040/freertos:~/.arduino15/packages/rp2040/hardware/rp2040/5.2.1/pico-sdk/src/boards/include/boards/:${pkgs.arduino.fhsenv}/usr/share/arduino/hardware/arduino/avr/cores/arduino/:${pkgs.arduino.fhsenv}/usr/share/arduino/hardware/tools/avr/avr/include:~/.arduino15/packages/rp2040/hardware/rp2040/5.2.1/variants/rpipico/:

#https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

  shellHook = ''
  export ARDUINO_CONFIG_FILE=$(pwd)/arduino-cli.yml
  export LD_LIBRARY_PATH=${pkgs.udev}/lib:$LD_LIBRARY_PATH
  alias upload='arduino-cli upload'
  alias compile='arduino-cli compile'
  '';
}
