
@echo off

pushd %~dp0..

cmake -S. -B"build-visual-studio"

popd

pause
