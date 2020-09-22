#!/bin/bash

GC=gps.conf
IC=izat.conf
LP=local.prop

GETTEXT=`adb root`
ROOTED="adbd is already running as root"
SUCCESS="restarting adbd as root"

REMOUNT=`adb remount | awk 'END{print}'`
RSUCCESS="remount succeeded"
FAILED=""

if [ "$GETTEXT" = "$ROOTED" ]; then
	echo "adbd is already running as root."
elif [ "$GETTEXT" = "$SUCCESS" ]; then
	echo "root success."
else
	echo "root failed."
	exit
fi

if [ "$REMOUNT" = "$RSUCCESS" ]; then
	echo "remount succeeded."
else
	echo "remount failed."
	exit
fi

adb pull /vendor/etc/$GC ./
adb pull /vendor/etc/$IC ./

if [ -e $GC ]; then
	sed -i '15d' ./$GC
	sed -i '14aDEBUG_LEVEL = 5' ./$GC
else
	echo "获取gps.conf文件失败。。。"
fi
			
			
if [ -e $IC ]; then
	sed -i '5d' ./$IC
	sed -i '4aIZAT_DEBUG_LEVEL = 5' ./$IC
else
	echo "获取izat.conf文件失败。。。"
fi
						
adb push $GC /vendor/etc/
adb push $IC /vendor/etc/

echo "log.tag.[XTRA2]XtraSubscriber=VERBOSE
log.tag.DataItemsFactoryProxy=VERBOSE
log.tag.DebugReportActivity=VERBOSE
log.tag.DebugReportCallbackInApp=VERBOSE
log.tag.DebugReportService=VERBOSE
log.tag.FlpHardwareProvider=VERBOSE
log.tag.FlpServiceProvider=VERBOSE
log.tag.FlpSvcProvider_jni=VERBOSE
log.tag.FrameworkActionRequest=VERBOSE
log.tag.FusedLocation=VERBOSE
log.tag.FusedLocationHardware=VERBOSE
log.tag.FusedProxy=VERBOSE
log.tag.GeofenceManager=VERBOSE
log.tag.GeofenceServiceProvider=VERBOSE
log.tag.GeofenceSvcProvider_jni=VERBOSE
log.tag.GNPProxy=VERBOSE
log.tag.GnssHal_GnssGeofencing=VERBOSE
log.tag.GnssLocationProvider=VERBOSE
log.tag.GpsXtraDownloader=VERBOSE
log.tag.GpsXtraDownloader-Q=VERBOSE
log.tag.InCall=VERBOSE
log.tag.IPCMessagingProxy=VERBOSE
log.tag.IZatManager=VERBOSE
log.tag.IzatProviderBase=VERBOSE
log.tag.IzatProviderEngine=VERBOSE
log.tag.IzatService=VERBOSE
log.tag.IzatServiceBase=VERBOSE
log.tag.IzatSvc_ComboNetworkProvider=VERBOSE
log.tag.IzatSvc_FreeWifiScanObserver=VERBOSE
log.tag.IzatSvc_GtpWiFiProxy=VERBOSE
log.tag.IzatSvc_GtpWwanProxy=VERBOSE
log.tag.IzatSvc_IzatManager=VERBOSE
log.tag.IzatSvc_LocationProvider=VERBOSE
log.tag.IzatSvc_OsNpProxy=VERBOSE
log.tag.IzatSvc_OSObserver=VERBOSE
log.tag.IzatSvc_PassiveLocListener=VERBOSE
log.tag.IzatSvc_PassiveNmeaListener=VERBOSE
log.tag.IzatSvc_PassiveSvInfoListener=VERBOSE
log.tag.IzatSvc_QNP=VERBOSE
log.tag.IzatSvc_Wiper=VERBOSE
log.tag.IzatSvc_ZppProxy=VERBOSE
log.tag.LBSSystemMonitorService=VERBOSE
log.tag.LocationManagerService=VERBOSE
log.tag.LocationServiceReceiver=VERBOSE
log.tag.LocSvc__AGnssRilInterface=VERBOSE
log.tag.LocSvc_afw=VERBOSE
log.tag.LocSvc_AGnssInterface=VERBOSE
log.tag.LocSvc_Agps=VERBOSE
log.tag.LocSvc_api_v02=VERBOSE
log.tag.LocSvc_APIClientBase=VERBOSE
log.tag.LocSvc_ApiV02=VERBOSE
log.tag.LocSvc_BatchingAPIClient=VERBOSE
log.tag.LocSvc_core_log=VERBOSE
log.tag.LocSvc_CtxBase=VERBOSE
log.tag.LocSvc_DualCtx=VERBOSE
log.tag.LocSvc_eng=VERBOSE
log.tag.LocSvc_EngAdapter=VERBOSE
log.tag.LocSvc_ext=VERBOSE
log.tag.locSvc_FlpAdapter=VERBOSE
log.tag.LocSvc_GeofenceApiClient=VERBOSE
log.tag.LocSvc_GnssAdapter=VERBOSE
log.tag.LocSvc_GnssAPIClient=VERBOSE
log.tag.LocSvc_GnssBatchingInterface=VERBOSE
log.tag.LocSvc_GnssConfigurationInterface=VERBOSE
log.tag.LocSvc_GnssDebugInterface=VERBOSE
log.tag.LocSvc_GnssInterface=VERBOSE
log.tag.LocSvc_GnssMeasurementInterface=VERBOSE
log.tag.LocSvc_GnssNiInterface=VERBOSE
log.tag.LocSvc_HIDL_AGnss=VERBOSE
log.tag.LocSvc_HIDL_DebugReportService=VERBOSE
log.tag.LocSvc_HIDL_DebugReportService_jni=VERBOSE
log.tag.LocSvc_HIDL_FlpClient=VERBOSE
log.tag.LocSvc_HIDL_FlpService=VERBOSE
log.tag.LocSvc_HIDL_FlpSvcProvider_jni=VERBOSE
log.tag.LocSvc_HIDL_GeofenceClient=VERBOSE
log.tag.LocSvc_HIDL_GeofenceService=VERBOSE
log.tag.LocSvc_HIDL_GeofenceSvcProvider_jni=VERBOSE
log.tag.LocSvc_HIDL_Gnss=VERBOSE
log.tag.LocSvc_HIDL_GnssNi=VERBOSE
log.tag.LocSvc_HIDL_IzatOsNpGlue=VERBOSE
log.tag.LocSvc_HIDL_IzatProvider=VERBOSE
log.tag.LocSvc_HIDL_IzatProvider_jni=VERBOSE
log.tag.LocSvc_HIDL_IzatSubscription=VERBOSE
log.tag.LocSvc_HIDL_IzatWiFiDBReceiver=VERBOSE
log.tag.LocSvc_HIDL_LocationService_jni=VERBOSE
log.tag.LocSvc_HIDL_NiClient=VERBOSE
log.tag.LocSvc_HIDL_OsNpGlue_jni=VERBOSE
log.tag.LocSvc_HIDL_RilInfoMonitor=VERBOSE
log.tag.LocSvc_HIDL_RilInfoMonitor_jni=VERBOSE
log.tag.LocSvc_HIDL_Subscription_jni=VERBOSE
log.tag.LocSvc_HIDL_Utils=VERBOSE
log.tag.LocSvc_HIDL_utils_jni=VERBOSE
log.tag.LocSvc_HIDL_WiFiDBReceiver_jni=VERBOSE
log.tag.LocSvc_HIDL_XT=VERBOSE
log.tag.LocSvc_IzatApiV02=VERBOSE
log.tag.LocSvc_java=VERBOSE
log.tag.LocSvc_jni=VERBOSE
log.tag.LocSvc_launcher=VERBOSE
log.tag.LocSvc_LBSAdapter=VERBOSE
log.tag.LocSvc_LBSApiBase=VERBOSE
log.tag.LocSvc_LBSApiV02=VERBOSE
log.tag.LocSvc_LBSProxy=VERBOSE
log.tag.LocSvc_libulp=VERBOSE
log.tag.LocSvc_LocAdapterBase=VERBOSE
log.tag.LocSvc_LocApiBase=VERBOSE
log.tag.LocSvc_LocationAPI=VERBOSE
log.tag.LocSvc_LocUlpProxy=VERBOSE
log.tag.LocSvc_MeasurementAPIClient=VERBOSE
log.tag.LocSvc_NiA=VERBOSE
log.tag.LocSvc_NiH=VERBOSE
log.tag.LocSvc_SystemStatus=VERBOSE
log.tag.LocSvc_SystemStatusOsObserver=VERBOSE
log.tag.LocSvc_xtra2=VERBOSE
log.tag.LocSvc_XtraSystemStatusObs=VERBOSE
log.tag.Mms=VERBOSE
log.tag.NetworkLocationProvider=VERBOSE
log.tag.NetworkLocationService=VERBOSE
log.tag.NpProxy=VERBOSE
log.tag.OsAgent=VERBOSE
log.tag.OSFramework=VERBOSE
log.tag.OSFrameworkCleaner=VERBOSE
log.tag.OsNpGlue=VERBOSE
log.tag.RawCommandInjector=VERBOSE
log.tag.RilInfoMonitor=VERBOSE
log.tag.SnapdragonSDKTest=VERBOSE
log.tag.Subscription=VERBOSE
log.tag.Telecomm=VERBOSE
log.tag.Telephony=VERBOSE
log.tag.UnifiedLocationProvider=VERBOSE
log.tag.UnifiedLocationService=VERBOSE
log.tag.WiFiDBReceiver=VERBOSE
log.tag.XTActivity=VERBOSE
log.tag.XTBroadcastReceiver=VERBOSE
log.tag.XTNative=VERBOSE
log.tag.XTNoticeActivity=VERBOSE
log.tag.XTRA_CLIENT=VERBOSE
log.tag.XTRA2=VERBOSE
log.tag.XTSettingInjectorSrv=VERBOSE
log.tag.XTSrv=VERBOSE
log.tag.XTVersionDownloader=VERBOSE
log.tag.LocSvc_NiA=VERBOSE
log.tag.NetworkLocationService=VERBOSE
log.tag.NpProxy=VERBOSE
log.tag.UnifiedLocationService=VERBOSE
log.tag.OsAgent=VERBOSE
log.tag.DataPerPackageAndUser=VERBOSE
log.tag.WiFiDBReceiver=VERBOSE
log.tag.LocationSettingsHelper=VERBOSE
log.tag.FlpServiceProvider=VERBOSE
log.tag.IzatService=VERBOSE
log.tag.DebugReportService=VERBOSE
log.tag.WWANDBReceiver=VERBOSE
log.tag.GeofenceServiceProvider=VERBOSE
log.tag.LocSvc_java=VERBOSE
log.tag.RilInfoMonitor=VERBOSE
log.tag.LocationServiceReceiver=VERBOSE
log.tag.LocSvc_NiH=VERBOSE
log.tag.GnssLocationProvider=VERBOSE
log.tag.LocationManagerService=VERBOSE
log.tag.GnssNetworkConnectivityHandler=VERBOSE " > $LP

adb push $LP /data/
						
adb shell chmod 644 /data/$LP
adb reboot
						
rm $GC
rm $IC
rm $LP
