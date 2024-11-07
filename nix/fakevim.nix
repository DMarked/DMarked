{ stdenv
, lib
, fetchFromGitHub
, cmake
, qtbase
}:

stdenv.mkDerivation {
  pname = "fakevim";
  version = "unstable-2022-09-29";

  src = fetchFromGitHub {
    owner = "hluk";
    repo = "FakeVim";
    rev = "f693878ed57c6cd31c028dc06d79dc962ee2fbc4";
    hash = "sha256-qLUfijlQcrtka7oMv8aVVy94/EbxLVCsPjPpWaoVB2E=";
  };

  nativeBuildInputs = [
    cmake
  ];

  dontWrapQtApps = true;

  buildInputs = [
    qtbase
  ];

  strictDeps = true;

  meta = with lib; {
    description = "Vim emulation in QTextEdit, QPlainTextEdit and similar Qt widgets";
    homepage = "https://github.com/hluk/FakeVim";
    license = licenses.lgpl2Plus;
    platforms = platforms.linux;
  };
}
