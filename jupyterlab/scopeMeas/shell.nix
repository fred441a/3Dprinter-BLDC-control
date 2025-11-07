{ pkgs ? import <nixpkgs> { } }:
let
  py = pkgs.python314;
  pythonEnv = py.withPackages (ps: [ ps.jupyterlab ps.numpy ps.scipy ps.sympy ]);
in pkgs.mkShell {
  buildInputs = [ pythonEnv pkgs.pandoc pkgs.texliveSmall ];
  shellHook = ''
    if [ ! -d .venv ]; then
      ${py.interpreter} -m venv .venv
    fi
    . .venv/bin/activate
    export PYTHONPATH="${pythonEnv}/${py.sitePackages}:$PYTHONPATH"
    export PIP_DISABLE_PIP_VERSION_CHECK=1
    export PYTHONNOUSERSITE=1
  '';
}
