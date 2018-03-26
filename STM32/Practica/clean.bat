echo off
del /s /q .dep
rmdir .dep
echo Directory del erased
del /s /q build
rmdir build\lst
rmdir build\obj
rmdir build
echo Directory build erased

