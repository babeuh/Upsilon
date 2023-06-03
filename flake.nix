{
  description = "Upsilon for Numworks";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
  let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {
    devShell.x86_64-linux =
      pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          pkg-config
        ];
        buildInputs = with pkgs; [
          libpng
          libjpeg
          freetype
          xorg.libX11
          xorg.libXext
          python3
          python310Packages.lz4
          python310Packages.pypng
          imagemagick
          gcc-arm-embedded
          gcc
        ];
      };
  };
}
