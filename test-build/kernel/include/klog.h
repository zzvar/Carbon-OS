/**
 * create on 2017/3/21
 * by wangchen11
 * [Carbon_OS team]
 */
enum KLOG_LEVEL{
	KLOG_LEVEL_I=0,
	KLOG_LEVEL_V,
	KLOG_LEVEL_D,
	KLOG_LEVEL_W,
	KLOG_LEVEL_E,
};

//#define LOGTAG "TAG"

#define KLOGI(f,args...) klog(KLOG_LEVEL_I,LOG_TAG,f,##args)
#define KLOGV(f,args...) klog(KLOG_LEVEL_V,LOG_TAG,f,##args)
#define KLOGD(f,args...) klog(KLOG_LEVEL_D,LOG_TAG,f,##args)
#define KLOGW(f,args...) klog(KLOG_LEVEL_W,LOG_TAG,f,##args)
#define KLOGE(f,args...) klog(KLOG_LEVEL_E,LOG_TAG,f,##args)

void klog(int level,const char *tag,const char *format,...);
