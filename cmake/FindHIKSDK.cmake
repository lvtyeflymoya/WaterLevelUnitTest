set (HIKSDK_DIR "F:/ThirdPart/Hikvision")

set (HIKSDK_INCLUDE_DIRS ${HIKSDK_DIR}/include/)

list (APPEND HIKSDK_LIBRARIES 
    ${HIKSDK_DIR}/lib/HCCore.lib
    ${HIKSDK_DIR}/lib/HCNetSDK.lib
    ${HIKSDK_DIR}/lib/GdiPlus.lib
    ${HIKSDK_DIR}/lib/HCAlarm.lib
    ${HIKSDK_DIR}/lib/HCGeneralCfgMgr.lib
    ${HIKSDK_DIR}/lib/HCPreview.lib
    ${HIKSDK_DIR}/lib/PlayCtrl.lib
)