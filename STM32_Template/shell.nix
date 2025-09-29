{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{
  packages = [
  stlink
  #stlink-gui
  gcc-arm-embedded
  ];
}
