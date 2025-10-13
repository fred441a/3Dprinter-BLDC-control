{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell{
  packages = with pkgs; [
  pandoc
  jupyter
  texliveTeTeX
  texliveSmall
  ];
  shellHook = "jupyter-lab";
}
