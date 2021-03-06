#include "HiVi.h"

typedef struct _vi_chn_map_{
    int phy_chn_num;
    VI_DEV vi_dev;
    VI_CHN primary_vi_chn;
}vi_chn_map_t;


CHiVi::CHiVi()
{
   _AV_FATAL_((m_pThread_lock = new CAvThreadLock) == NULL, "OUT OF MEMORY\n");
   m_u8LastSatu = 32;
   m_u8LastColorGrayMode = 0;
   m_tv_system =  _AT_UNKNOWN_;
}


CHiVi::~CHiVi()
{
    _AV_SAFE_DELETE_(m_pThread_lock);
}



#if defined(_AV_PLATFORM_HI3531_) || defined(_AV_PLATFORM_HI3532_)
int CHiVi::HiVi_vi_dev_control_3531_v1(bool start_flag)
{
    std::bitset<_MAX_VI_DEV_NUM_> vi_dev_state;
    HiVi_get_vi_dev_info(vi_dev_state);

    /*DEV2;VIU0[7:0];[BNC:CH1, BNC:CH4]*/
    if (vi_dev_state.test(2))
    {
        if(HiVi_vi_dev_control_V1(2, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0x00ff0000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3531_v1 HiVi_vi_dev_control_V1[DEV2] FAILED\n");
            return -1;
        }
    }

    /*DEV0;VIU0[15:8];[BNC:CH5, BNC:CH8]*/
    if (vi_dev_state.test(0))
    {
        if(HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0xff000000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3531_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
            return -1;
        }
    }

    /*DEV6;VIU2[15:8];[BNC:CH9, BNC:CH12]*/
    if (vi_dev_state.test(6))
    {
        if(HiVi_vi_dev_control_V1(6, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0x00ff0000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3531_v1 HiVi_vi_dev_control_V1[DEV6] FAILED\n");
            return -1;
        }
    }

    /*DEV4;VIU2[7:0];[BNC:CH13, BNC:CH16]*/   
    if (vi_dev_state.test(4))
    {
        if(HiVi_vi_dev_control_V1(4, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0xff000000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3531_v1 HiVi_vi_dev_control_V1[DEV4] FAILED\n");
            return -1;
        }
    }

    return 0;
}
#endif

#if defined(_AV_PLATFORM_HI3521_)
int CHiVi::HiVi_vi_dev_control_3521_v1(bool start_flag)
{
    std::bitset<_MAX_VI_DEV_NUM_> vi_dev_state;
    HiVi_get_vi_dev_info(vi_dev_state);

    if (vi_dev_state.test(0))
    {
        if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0xff000000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
            return -1;
        }
    }
    if (vi_dev_state.test(1))
    {
        if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0x00ff0000) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV1] FAILED\n");
            return -1;
        }
    }
    return 0;
}
#endif

#if defined(_AV_PLATFORM_HI3520D_)||defined(_AV_PLATFORM_HI3520D_V300_)
int CHiVi::HiVi_vi_dev_control_3520D_v1(bool start_flag)
{
    std::bitset<_MAX_VI_DEV_NUM_> vi_dev_state;
    HiVi_get_vi_dev_info(vi_dev_state);
	
    eProductType product_type = PTD_INVALID;
    eSystemType subproduct_type = SYSTEM_TYPE_MAX;
    product_type = pgAvDeviceInfo->Adi_product_type();
    subproduct_type = pgAvDeviceInfo->Adi_sub_product_type();

		if(((product_type == PTD_X1_AHD) && ((subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400_AHD)))
		||((product_type == PTD_D5) && (subproduct_type == SYSTEM_TYPE_D5_AHD))
		||((product_type == PTD_X3) && (subproduct_type == SYSTEM_TYPE_X3_AHD)))
	{
		        if((vi_dev_state.test(0))&&((pgAvDeviceInfo->Adi_videoformat_isahd(2) != 0)||(pgAvDeviceInfo->Adi_videoformat_isahd(3) != 0)))
			{
			    	printf("HiVi_vi_dev_control_3520D_V200_v1 004 scanmode:VI_SCAN_PROGRESSIVE,start_flag=%d\n",start_flag);
			       if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff000000) != 0)
			        {
			            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
			            return -1;
			        }
			}
			else
			{
			    	printf("HiVi_vi_dev_control_3520D_V200_v1 005 scanmode:VI_SCAN_INTERLACED,start_flag=%d\n",start_flag);
			       if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_INTERLACED, 0xff000000) != 0)
			        {
			            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
			            return -1;
			        }
			}

		        if((vi_dev_state.test(1))&&((pgAvDeviceInfo->Adi_videoformat_isahd(0) != 0)||(pgAvDeviceInfo->Adi_videoformat_isahd(1) != 0)))
			{
			    	printf("HiVi_vi_dev_control_3520D_V200_v1 007 scanmode:VI_SCAN_PROGRESSIVE,start_flag=%d\n",start_flag);
			       if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff0000) != 0)
			        {
			            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
			            return -1;
			        }
			 }
			 else
			 {
			    	printf("HiVi_vi_dev_control_3520D_V200_v1 008 scanmode:VI_SCAN_INTERLACED,start_flag=%d\n",start_flag);
			       if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_INTERLACED, 0xff0000) != 0)
			        {
			            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
			            return -1;
			        }
			  }

	}
	else if((product_type == PTD_XMD3104)||(product_type == PTD_ES4206)||(product_type == PTD_M0026)||\
        (product_type == PTD_M3))
	{
		if (vi_dev_state.test(0))
	       {
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 000,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }
	       }						
		if(vi_dev_state.test(1))
		{
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 002,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff00) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV1] FAILED\n");
		            return -1;
		        }    
		}		
	}
	else if(product_type == PTD_ES8412 || product_type == PTD_6A_II_AVX || product_type == PTD_A5_AHD)	
	{
		if (vi_dev_state.test(0))
	       {
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 170,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }
	       }						
		if(vi_dev_state.test(1))
		{
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 171,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff00) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }    
		}
		if(vi_dev_state.test(2))
		{
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 172,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(2, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff0000) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }    
		}
		if(vi_dev_state.test(3))
		{
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 173,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(3, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff000000) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }    
		}		
	}	
	else if(product_type == PTD_D5_35)
	{
		if (vi_dev_state.test(0))
	       {
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 010,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_PROGRESSIVE,0xff) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }
	       }						
		if(vi_dev_state.test(1))
		{
		    	printf("HiVi_vi_dev_control_3520D_V200_v1 011,start_flag=%d\n",start_flag);
		       if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_2Multiplex, VI_SCAN_PROGRESSIVE, 0xff00) != 0)
		        {
		            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
		            return -1;
		        }    
		}
	}
	else if(product_type == PTD_P1)
	{
		//经测试，发现P1只有两个vi，可以打开dev0和dev1，其他的不行，因此音频通道从第2路开始算起，后续需要修改。///
		if (vi_dev_state.test(0))
		{
			if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_PROGRESSIVE,0xff) != 0)
			{
				DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
				return -1;
			}
		}						
		if(vi_dev_state.test(1))
		{
			if (HiVi_vi_dev_control_V1(1, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_PROGRESSIVE, 0xff00) != 0)
			{
				DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
				return -1;
			}    
		}
	}
	else
	{
	    if (vi_dev_state.test(0))
	    {
	        if (HiVi_vi_dev_control_V1(0, start_flag, VI_MODE_BT656, VI_WORK_MODE_4Multiplex, VI_SCAN_INTERLACED, 0xff000000) != 0)
	        {
	            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_3521_v1 HiVi_vi_dev_control_V1[DEV0] FAILED\n");
	            return -1;
	        }
	    }
	}
    return 0;
}
#endif

#if defined(_AV_PLATFORM_HI3518C_)
int CHiVi::HiVi_vi_dev_control_3518_v1(bool start_flag)
{
    std::bitset<_MAX_VI_DEV_NUM_> vi_dev_state;
    HiVi_get_vi_dev_info(vi_dev_state);

    if(HiVi_vi_dev_control_V2(0, start_flag, VI_MODE_DIGITAL_CAMERA, VI_WORK_MODE_1Multiplex, VI_SCAN_PROGRESSIVE, 0xFFF00000) != 0)
    {
        _AV_KEY_INFO_(_AVD_VI_, "CHiVi::HiVi_vi_dev_control_711A1_VA HiVi_vi_dev_control_V1[DEV1] FAILED\n");
        return -1;
    }

    return 0;
}
#endif
int CHiVi::HiVi_all_vi_dev_control(bool start_flag)
{
#if defined(_AV_PLATFORM_HI3531_) || defined(_AV_PLATFORM_HI3532_)
    return HiVi_vi_dev_control_3531_v1(start_flag);
#elif defined(_AV_PLATFORM_HI3521_)
    return HiVi_vi_dev_control_3521_v1(start_flag);
#elif defined(_AV_PLATFORM_HI3520D_) || defined(_AV_PLATFORM_HI3520D_V300_)
	return HiVi_vi_dev_control_3520D_v1(start_flag);
#elif defined(_AV_PLATFORM_HI3518C_)
#if defined(_AV_PRODUCT_CLASS_IPC_)
    return  HiVi_vi_dev_control_3518_v1(start_flag);
#endif
#else
    _AV_FATAL_(1, "you must fucking select a platform!\n");
    return -1;
#endif
}

int CHiVi::HiVi_get_video_size(vi_config_set_t *pConfig, int *pWidth, int *pHeight)
{
    switch(pConfig->resolution)
    {
        case _AR_SIZE_:
            _AV_POINTER_ASSIGNMENT_(pWidth, pConfig->w);
            _AV_POINTER_ASSIGNMENT_(pHeight, pConfig->h);
            break;

       default:
            return pgAvDeviceInfo->Adi_get_video_size(pConfig->resolution, m_tv_system, pWidth, pHeight);
//            break;
    }

    return 0;
}

#ifdef _AV_PLATFORM_HI3520D_V300_
VI_DEV_ATTR_S DEV_ATTR_6114_720P_2MUX_BASE =
{
    /*接口模式*/
    VI_MODE_BT656,
    /*1、2、4路工作模式*/
    VI_WORK_MODE_2Multiplex,
    /* r_mask    g_mask    b_mask*/
    {0xFF000000,    0x0},

	/* 双沿输入时必须设置 */
	VI_CLK_EDGE_SINGLE_UP,
	
    /*AdChnId*/
    {-1, -1, -1, -1},
    /*enDataSeq, 仅支持YUV格式*/
    VI_INPUT_DATA_YVYU,
    /*同步信息，对应reg手册的如下配置, --bt1120时序无效*/
    {
    /*port_vsync   port_vsync_neg     port_hsync        port_hsync_neg        */
    VI_VSYNC_FIELD, VI_VSYNC_NEG_HIGH, VI_HSYNC_VALID_SINGNAL,VI_HSYNC_NEG_HIGH,VI_VSYNC_VALID_SINGAL,VI_VSYNC_VALID_NEG_HIGH,

    /*timing信息，对应reg手册的如下配置*/
    /*hsync_hfb    hsync_act    hsync_hhb*/
    {0,            0,        0,
    /*vsync0_vhb vsync0_act vsync0_hhb*/
     0,            0,        0,
    /*vsync1_vhb vsync1_act vsync1_hhb*/
     0,            0,            0}
    },
    /*使用内部ISP*/
    VI_PATH_BYPASS,
    /*输入数据类型*/
    VI_DATA_TYPE_YUV
};
#endif
int CHiVi::HiVi_vi_dev_control_V1(VI_DEV vi_dev, bool start_flag, VI_INTF_MODE_E inte_mode/* = VI_MODE_BUTT*/, VI_WORK_MODE_E work_mode/* = VI_WORK_MODE_BUTT*/, VI_SCAN_MODE_E scan_mode/* = VI_SCAN_BUTT*/, HI_U32 mask_0/* = 0*/, HI_U32 mask_1/* = 0*/)    
{
    VI_DEV_ATTR_S vi_dev_attr;

    if(start_flag == true)
    {
        memset(&vi_dev_attr, 0, sizeof(VI_DEV_ATTR_S));

#ifdef _AV_PLATFORM_HI3520D_V300_
        memcpy(&vi_dev_attr,&DEV_ATTR_6114_720P_2MUX_BASE,sizeof(vi_dev_attr));
#else
        vi_dev_attr.enScanMode = scan_mode;
#endif  
        vi_dev_attr.enIntfMode = inte_mode;
        vi_dev_attr.enWorkMode = work_mode;
        vi_dev_attr.s32AdChnId[0] = -1;
        vi_dev_attr.s32AdChnId[1] = -1;
        vi_dev_attr.s32AdChnId[2] = -1;
        vi_dev_attr.s32AdChnId[3] = -1;
        vi_dev_attr.au32CompMask[0] = mask_0;
        vi_dev_attr.au32CompMask[1] = mask_1;
        
#if defined(_AV_PLATFORM_HI3520D_)||defined(_AV_PLATFORM_HI3520D_V300_)
	eProductType product_type = PTD_INVALID;
	eSystemType subproduct_type = SYSTEM_TYPE_MAX;
	product_type = pgAvDeviceInfo->Adi_product_type();
	subproduct_type = pgAvDeviceInfo->Adi_sub_product_type();
	
	 if(((product_type == PTD_X1_AHD) && ((subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400_AHD)))
		||((product_type == PTD_X1) && ((subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401)))
		||((product_type == PTD_D5) && (subproduct_type == SYSTEM_TYPE_D5_AHD))
		||(product_type == PTD_D5M))
		{
			//反向连接
        	vi_dev_attr.bDataRev = HI_TRUE;
			DEBUG_ERROR("反向链接\n");
    	}
	if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)	
	{
#if defined(_AV_PLATFORM_HI3520D_V300_)
		vi_dev_attr.enClkEdge = VI_CLK_EDGE_DOUBLE;
#endif
	}
#endif

        if(HiVi_start_vi_dev(vi_dev, &vi_dev_attr) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_V1 HiVi_start_vi_dev[DEV%d] FAILED\n", vi_dev);
            return -1;
        }
    }
    else
    {
        if(HiVi_stop_vi_dev(vi_dev) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_V1 HiVi_stop_vi_dev[DEV%d] FAILED\n", vi_dev);
            return -1;
        }
    }

    return 0;
}

#if defined(_AV_PLATFORM_HI3518C_)
int CHiVi::HiVi_vi_dev_control_V2(VI_DEV vi_dev, bool start_flag, VI_INTF_MODE_E inte_mode/* = VI_MODE_BUTT*/, VI_WORK_MODE_E work_mode/* = VI_WORK_MODE_BUTT*/, VI_SCAN_MODE_E scan_mode/* = VI_SCAN_BUTT*/, HI_U32 mask_0/* = 0*/, HI_U32 mask_1/* = 0*/)    
{
    VI_DEV_ATTR_S vi_dev_attr;

    if(start_flag == true)
    {
        memset(&vi_dev_attr, 0, sizeof(VI_DEV_ATTR_S));

        vi_dev_attr.enIntfMode = inte_mode;
        vi_dev_attr.enWorkMode = work_mode;
        vi_dev_attr.enScanMode = scan_mode;
        vi_dev_attr.s32AdChnId[0] = -1;
        vi_dev_attr.s32AdChnId[1] = -1;
        vi_dev_attr.s32AdChnId[2] = -1;
        vi_dev_attr.s32AdChnId[3] = -1;
        vi_dev_attr.au32CompMask[0] = mask_0;
        vi_dev_attr.au32CompMask[1] = mask_1;

        sensor_e st  = ST_SENSOR_DEFAULT;
        unsigned char chn_id = 0;
        vi_capture_rect_s rect;

        memset(&rect, 0, sizeof(vi_capture_rect_s));
        if(PTD_712_VD == pgAvDeviceInfo->Adi_product_type())
        {
            if(0 != pgAvDeviceInfo->Adi_get_ipc_work_mode())
            {
                chn_id = 1;
            }
        }
        st = pgAvDeviceInfo->Adi_get_sensor_type();
        pgAvDeviceInfo->Adi_get_vi_config(st, chn_id, pgAvDeviceInfo->Adi_get_vi_max_resolution(), rect);
        
        switch(pgAvDeviceInfo->Adi_product_type())
        {
            case PTD_712_VD:
            case PTD_712_VA:
            case PTD_712_VB:
            case PTD_712_VC:
            case PTD_714_VA:
            case PTD_712_VE:
            case PTD_712_VF:
            case PTD_714_VB:   
            default:
            {
                //720p
                vi_dev_attr.enDataPath = VI_PATH_ISP; 
                vi_dev_attr.enInputDataType = VI_DATA_TYPE_RGB;
                vi_dev_attr.enDataSeq = VI_INPUT_DATA_YUYV;
                
                vi_dev_attr.stSynCfg.enVsync = VI_VSYNC_PULSE;
                vi_dev_attr.stSynCfg.enVsyncNeg = VI_VSYNC_NEG_HIGH;
                vi_dev_attr.stSynCfg.enHsync = VI_HSYNC_VALID_SINGNAL;
                vi_dev_attr.stSynCfg.enHsyncNeg = VI_HSYNC_NEG_HIGH;
                vi_dev_attr.stSynCfg.enVsyncValid = VI_VSYNC_VALID_SINGAL;
                
                vi_dev_attr.stSynCfg.enVsyncValidNeg = VI_VSYNC_VALID_NEG_HIGH;
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncHfb = 0;    
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncAct = rect.m_vi_capture_width;    
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncHbb = 0;  
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVfb = 0;
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVact = rect.m_vi_capture_height;

                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbb = 0;   
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbfb = 0;   
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbact = 0;  
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbbb = 0; 
            }
                break;
            case PTD_913_VA:
            {
                //1080p
                vi_dev_attr.enDataSeq = VI_INPUT_DATA_YUYV;
                
                vi_dev_attr.stSynCfg.enVsync = VI_VSYNC_PULSE;
                vi_dev_attr.stSynCfg.enVsyncNeg = VI_VSYNC_NEG_HIGH;
                vi_dev_attr.stSynCfg.enHsync = VI_HSYNC_VALID_SINGNAL;
                vi_dev_attr.stSynCfg.enHsyncNeg = VI_HSYNC_NEG_HIGH;
                vi_dev_attr.stSynCfg.enVsyncValid = VI_VSYNC_NORM_PULSE;
                vi_dev_attr.stSynCfg.enVsyncValidNeg = VI_VSYNC_VALID_NEG_HIGH;
                
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncHfb = 0;    
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncAct = rect.m_vi_capture_width;    
                vi_dev_attr.stSynCfg.stTimingBlank.u32HsyncHbb = 0;  
                
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVfb = 0;  
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVact = rect.m_vi_capture_height;
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbb = 0;   
                
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbfb = 0;   
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbact = 0;  
                vi_dev_attr.stSynCfg.stTimingBlank.u32VsyncVbbb = 0; 
                
                vi_dev_attr.enDataPath = VI_PATH_ISP; 
                vi_dev_attr.enInputDataType = VI_DATA_TYPE_RGB;

            }
                break;
        }
        
        
        if(HiVi_start_vi_dev(vi_dev, &vi_dev_attr) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_V1 HiVi_start_vi_dev[DEV%d] FAILED\n", vi_dev);
            return -1;
        }
    }
    else
    {
        if(HiVi_stop_vi_dev(vi_dev) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_vi_dev_control_V1 HiVi_stop_vi_dev[DEV%d] FAILED\n", vi_dev);
            return -1;
        }
    }

    return 0;
}
#endif

int CHiVi::HiVi_start_vi_dev(VI_DEV vi_dev, const VI_DEV_ATTR_S *pVi_dev_attr)
{
    HI_S32 ret_val = -1;
    //autoly set bind relation between DEV and CHN
    if((ret_val = HI_MPI_VI_SetDevAttr(vi_dev, pVi_dev_attr)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_dev HI_MPI_VI_SetDevAttr FAILED(vi_dev:%d)(0x%08lx)\n", vi_dev, (unsigned long)ret_val);
        return -1;
    }
#ifdef _AV_PLATFORM_HI3520D_
	if(pVi_dev_attr->enScanMode == VI_SCAN_PROGRESSIVE)
	{
		HI_S32 s32Ret = HI_MPI_VI_SetPortMode(vi_dev, VI_PORTMODE_720P);
	    if (s32Ret != HI_SUCCESS)
	    {
	        DEBUG_ERROR("HI_MPI_VI_SetPortMode failed with %#x!\n", s32Ret);
	        return HI_FAILURE;
	    }
	}
#endif	
    if((ret_val = HI_MPI_VI_EnableDev(vi_dev)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_dev HI_MPI_VI_EnableDev FAILED(vi_dev:%d)(0x%08lx)\n", vi_dev, (unsigned long)ret_val);
        return -1;
    }

    return 0;
}


int CHiVi::HiVi_stop_vi_dev(VI_DEV vi_dev)
{
    HI_S32 ret_val = HI_MPI_VI_DisableDev(vi_dev);

    if(ret_val != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_stop_vi_dev HI_MPI_VI_DisableDev FAILED(vi_dev:%d)(0x%08lx)\n", vi_dev, (unsigned long)ret_val);
        return -1;
    }

    return 0;
}

int CHiVi::HiVi_start_vi_chn(int phy_chn_num, VI_CHN vi_chn, const VI_CHN_ATTR_S *pVi_chn_attr, unsigned char rotate/*=0*/)
{
    HI_S32 ret_val = -1;
#if defined(_AV_PLATFORM_HI3531_) || defined(_AV_PLATFORM_HI3532_)
    const char *pMemory_name = NULL;
    MPP_CHN_S mpp_chn;

    if(((vi_chn + 1) % 3) != 0)
    {
        pMemory_name = "ddr1";
    }
    mpp_chn.enModId  = HI_ID_VIU;
    mpp_chn.s32DevId = 0;
    mpp_chn.s32ChnId = vi_chn;
    if((ret_val = HI_MPI_SYS_SetMemConf(&mpp_chn, pMemory_name)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_chn HI_MPI_SYS_SetMemConf FAILED(0x%08lx)\n", (unsigned long)ret_val);
    }
#endif

#if defined(_AV_PLATFORM_HI3520D_)||defined(_AV_PLATFORM_HI3520D_V300_)
	HI_S32 s32Ret;
	VI_CHN_BIND_ATTR_S stChnBindAttr;
	eProductType product_type = PTD_INVALID;
	product_type = pgAvDeviceInfo->Adi_product_type();
#endif

#if defined(_AV_PLATFORM_HI3520D_)
       eSystemType subproduct_type = SYSTEM_TYPE_MAX;
	subproduct_type = pgAvDeviceInfo->Adi_sub_product_type();
	if(((2 == vi_chn) && (product_type == PTD_X3) && (subproduct_type == SYSTEM_TYPE_X3_AHD))
		||((2 == vi_chn) && (product_type == PTD_X1_AHD) &&( (subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400_AHD)))
		||((2 == vi_chn) && (product_type == PTD_D5) && (subproduct_type == SYSTEM_TYPE_D5_AHD)))
	{
		s32Ret = HI_MPI_VI_DisableChn(vi_chn);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_DisableChn failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}				
		s32Ret = HI_MPI_VI_ChnUnBind(vi_chn);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_ChnUnBind failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
		stChnBindAttr.ViDev = 0;
		stChnBindAttr.ViWay = 1;
		s32Ret = HI_MPI_VI_ChnBind(vi_chn, &stChnBindAttr);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_ChnBind failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}			
	}
	else if(((6 == vi_chn) && (product_type == PTD_X3) && (subproduct_type == SYSTEM_TYPE_X3_AHD))
		||((6 == vi_chn) && (product_type == PTD_X1_AHD) && ((subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401_AHD)
		||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400_AHD)))
		||((6 == vi_chn) && (product_type == PTD_D5) && (subproduct_type == SYSTEM_TYPE_D5_AHD)))
	{
		s32Ret = HI_MPI_VI_DisableChn(vi_chn);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_DisableChn failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
		s32Ret = HI_MPI_VI_ChnUnBind(vi_chn);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_ChnUnBind failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}
		stChnBindAttr.ViDev = 1;
		stChnBindAttr.ViWay = 1;
		s32Ret = HI_MPI_VI_ChnBind(vi_chn, &stChnBindAttr);
		if (s32Ret != HI_SUCCESS)
		{
			DEBUG_ERROR("HI_MPI_VI_ChnBind failed with %#x!\n", s32Ret);
			return HI_FAILURE;
		}			
	}			
#endif 

#ifdef _AV_PLATFORM_HI3520D_V300_
	if(((product_type == PTD_XMD3104)||(product_type == PTD_M0026)||(product_type == PTD_ES4206)||\
        (product_type == PTD_ES8412) || (product_type == PTD_6A_II_AVX) || (product_type == PTD_A5_AHD)||\
        (product_type == PTD_M3))
		&&((2 == vi_chn)||(6 == vi_chn)||(10 == vi_chn)||(14 == vi_chn)))
	{
                s32Ret = HI_MPI_VI_GetChnBind(vi_chn, &stChnBindAttr);
                if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
                {
                    stChnBindAttr.ViDev = vi_chn/4;
                    stChnBindAttr.ViWay = 1;
                    s32Ret = HI_MPI_VI_BindChn(vi_chn, &stChnBindAttr);
                    if (HI_SUCCESS != s32Ret)
                    {
                        DEBUG_ERROR("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
                        return HI_FAILURE;
                    } 
                } 
	}
	if((product_type == PTD_D5_35) &&((4 == vi_chn)||(6 == vi_chn)))
	{
                s32Ret = HI_MPI_VI_GetChnBind(vi_chn, &stChnBindAttr);
		printf("\ns32Ret=%x,HI_ERR_VI_FAILED_NOTBIND=%x,stChnBindAttr.ViDev=%d\n",
			s32Ret,HI_ERR_VI_FAILED_NOTBIND,stChnBindAttr.ViDev);
               if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
                {
                    stChnBindAttr.ViDev = vi_chn/4;;
                    stChnBindAttr.ViWay = 1;
			printf("\nvi_chn=%d,stChnBindAttr.ViDev=%d,stChnBindAttr.ViWay=%d\n",vi_chn,stChnBindAttr.ViDev,stChnBindAttr.ViWay);		
                    s32Ret = HI_MPI_VI_BindChn(vi_chn, &stChnBindAttr);
                    if (HI_SUCCESS != s32Ret)
                    {
                        DEBUG_ERROR("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
                        return HI_FAILURE;
                    } 
                } 
	}
	if((product_type == PTD_P1) && (vi_chn >= 0 && vi_chn <= 7))
	{
		s32Ret = HI_MPI_VI_GetChnBind(vi_chn, &stChnBindAttr);
		
		if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
		{
			stChnBindAttr.ViDev = vi_chn/4;
			stChnBindAttr.ViWay = 1;
			DEBUG_CRITICAL("\nP1:vi_chn=%d,stChnBindAttr.ViDev=%d,stChnBindAttr.ViWay=%d\n",vi_chn,stChnBindAttr.ViDev,stChnBindAttr.ViWay);		
			s32Ret = HI_MPI_VI_BindChn(vi_chn, &stChnBindAttr);
			if (HI_SUCCESS != s32Ret)
			{
				DEBUG_ERROR("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
				return HI_FAILURE;
			} 
		} 
	}
	/*if((product_type == PTD_M0026) && (vi_chn == 0 || vi_chn == 2 || vi_chn == 4 || vi_chn == 6))
	{
		s32Ret = HI_MPI_VI_GetChnBind(vi_chn, &stChnBindAttr);
		
		if (HI_ERR_VI_FAILED_NOTBIND == s32Ret)
		{
			stChnBindAttr.ViDev = vi_chn/4;
			stChnBindAttr.ViWay = 1;
			DEBUG_CRITICAL("M0026:vi_chn=%d,stChnBindAttr.ViDev=%d,stChnBindAttr.ViWay=%d\n",vi_chn,stChnBindAttr.ViDev,stChnBindAttr.ViWay);		
			s32Ret = HI_MPI_VI_BindChn(vi_chn, &stChnBindAttr);
			if (HI_SUCCESS != s32Ret)
			{
				DEBUG_ERROR("call HI_MPI_VI_BindChn failed with %#x\n", s32Ret);
				return HI_FAILURE;
			} 
		} 
	}*/
#endif	

#if defined(_AV_PLATFORM_HI3520D_)||defined(_AV_PLATFORM_HI3520D_V300_)
	s32Ret = HI_MPI_VI_DisableChn(vi_chn);
	if (s32Ret != HI_SUCCESS)
	{
		DEBUG_ERROR("HI_MPI_VI_DisableChn failed with %#x!\n", s32Ret);
		return HI_FAILURE;
	}
#endif
		
	DEBUG_CRITICAL( "CHiVi::HiVi_start_vi_chn,vi_chn(%d), stCapRect(%d, %d, %d, %d), stDestSize (%d, %d),src_fr %d\n", vi_chn,
            pVi_chn_attr->stCapRect.s32X, pVi_chn_attr->stCapRect.s32Y, pVi_chn_attr->stCapRect.u32Width, pVi_chn_attr->stCapRect.u32Height,
            pVi_chn_attr->stDestSize.u32Width, pVi_chn_attr->stDestSize.u32Height,
           pVi_chn_attr->s32SrcFrameRate);
    if((ret_val = HI_MPI_VI_SetChnAttr(vi_chn, pVi_chn_attr)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_chn stCapRect(%d, %d, %d, %d), stDestSize (%d, %d),src_fr %d\n", 
            pVi_chn_attr->stCapRect.s32X, pVi_chn_attr->stCapRect.s32Y, pVi_chn_attr->stCapRect.u32Width, pVi_chn_attr->stCapRect.u32Height,
            pVi_chn_attr->stDestSize.u32Width, pVi_chn_attr->stDestSize.u32Height,
           pVi_chn_attr->s32SrcFrameRate);

        DEBUG_ERROR( "CHiVi::HiVi_start_vi_chn HI_MPI_VI_SetChnAttr FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return -1;
    }

#ifdef _AV_PLATFORM_HI3520D_
	if((pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num) != 0))
	{
		if((vi_chn == 2) || (vi_chn == 6)) 
		{
			s32Ret = HI_MPI_VI_EnableChn422to420(vi_chn);
			if (s32Ret != HI_SUCCESS)
			{
				DEBUG_ERROR("HI_MPI_VI_EnableChn422to420 failed with %#x!\n", s32Ret);
				return HI_FAILURE;
			}	
		}
	}
	DEBUG_CRITICAL("\nkkkkkkk,phy_chn_num:(%d),HiVi_videoformat_isahd(%d)\n",phy_chn_num,pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num));	
#endif		
    if((ret_val = HI_MPI_VI_EnableChn(vi_chn)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_chn HI_MPI_VI_EnableChn FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return -1;
    }

    ret_val = HI_MPI_VI_SetFrameDepth( vi_chn, 1 );
    if( HI_SUCCESS != ret_val )
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_vi_chn HI_MPI_VI_SetFrameDepth FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return HI_FAILURE;
    }

    return 0;
}


int CHiVi::HiVi_stop_vi_chn(VI_CHN vi_chn)
{
    HI_S32 ret_val = -1; 

    if(HI_SUCCESS != (ret_val = HI_MPI_VI_SetFrameDepth(vi_chn, 0)))
    {
        DEBUG_ERROR( "CHiVi::HiVi_stop_vi_chn HI_MPI_VI_SetFrameDepth FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return -1;
    }
#if defined(_AV_PLATFORM_HI3520D_V300_)
    if(HI_SUCCESS != (ret_val = HI_MPI_VI_DisableUserPic(vi_chn)))
    {
        DEBUG_ERROR( "CHiVi::HiVi_stop_vi_chn HI_MPI_VI_DisableUserPic FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return -1;
    }	
#endif
    if(HI_SUCCESS != (ret_val = HI_MPI_VI_DisableChn(vi_chn)))
    {
        DEBUG_ERROR( "CHiVi::HiVi_stop_vi_chn HI_MPI_VI_DisableChn FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        return -1;
    }

    return 0;
}


int CHiVi::HiVi_start_all_vi_dev()
{
    int ret_val = -1;

    m_pThread_lock->lock();

    ret_val = HiVi_all_vi_dev_control(true);

    m_pThread_lock->unlock();

    return ret_val;
}


int CHiVi::HiVi_stop_all_vi_dev()
{
    int ret_val = -1;

    m_pThread_lock->lock();
    
    ret_val = HiVi_all_vi_dev_control(false);

    m_pThread_lock->unlock();

    return ret_val;

}


int CHiVi::HiVi_get_minor_vi_info(int phy_chn_num, VI_DEV *pVi_dev/* = NULL*/, VI_CHN *pVi_chn/* = NULL*/)
{
    VI_DEV vi_dev;
    VI_CHN vi_chn;

    if(HiVi_get_primary_vi_info(phy_chn_num, &vi_dev, &vi_chn) != 0)
    {
        return -1;
    }
    _AV_POINTER_ASSIGNMENT_(pVi_dev, vi_dev);
    _AV_POINTER_ASSIGNMENT_(pVi_chn, SUBCHN(vi_chn));

    return 0;
}

int CHiVi::HiVi_get_vi_dev_info(std::bitset<_MAX_VI_DEV_NUM_> &vi_dev)
{
    vi_dev.reset();
    for (int i = 0; i < pgAvDeviceInfo->Adi_get_vi_chn_num(); i++)
    {
        VI_DEV Vi_dev = -1;
        VI_CHN Vi_chn = -1;

        if (HiVi_get_primary_vi_info(i, &Vi_dev, &Vi_chn) < 0)
        {
            continue;
        }
        if ((Vi_dev >= 0) && (Vi_dev < _MAX_VI_DEV_NUM_))
        {
            vi_dev[Vi_dev] = 1;
        }
    }
    return 0;
}

int CHiVi::HiVi_get_primary_vi_info(int phy_chn_num, VI_DEV *pVi_dev/* = NULL*/, VI_CHN *pVi_chn/* = NULL*/, av_rect_t *pRect/* = NULL*/)
{
    vi_chn_map_t *pVi_chn_map = NULL;
    int index = 0;

    //不同的硬件设计，通道对应的vi设备和vi通道不一样，以下针对不同的硬件设计为pVi_chn_map赋值
#if defined(_AV_PLATFORM_HI3531_)
    static vi_chn_map_t hisi3531_vi_chn_map_v1[] = {
    {0, 2, 4}, {1, 2, 5}, {2, 2, 6}, {3, 2, 7},
    {4, 0, 0}, {5, 0, 1}, {6, 0, 2}, {7, 0, 3}, 
    {8, 6, 12}, {9, 6, 13}, {10, 6, 14}, {11, 6, 15}, 
    {12, 4, 8}, {13, 4, 9}, {14, 4, 10}, {15, 4, 11},
    {-1, 0, 0}};

    pVi_chn_map = hisi3531_vi_chn_map_v1;
#elif defined(_AV_PLATFORM_HI3521_)
    static vi_chn_map_t hisi3521_vi_chn_map_v1[] = {
    {0, 1, 4}, {1, 1, 5}, {2, 1, 6}, {3, 1, 7},
    {4, 0, 0}, {5, 0, 1}, {6, 0, 2}, {7, 0, 3}, 
    {-1, 0, 0}};

    pVi_chn_map = hisi3521_vi_chn_map_v1;
#elif defined(_AV_PLATFORM_HI3520D_)||defined(_AV_PLATFORM_HI3520D_V300_)
    vi_chn_map_t *hisi3520d_vi_chn_map_v1 = NULL;
    eProductType product_type = PTD_INVALID;
    eSystemType subproduct_type = SYSTEM_TYPE_MAX;
    product_type = pgAvDeviceInfo->Adi_product_type();
    subproduct_type = pgAvDeviceInfo->Adi_sub_product_type();

	static vi_chn_map_t hisi3521a_vi_chn_map_v1_sd[] = {
    {4, 1, 4}, {5, 1, 5}, {6, 1, 6}, {7, 1, 7},
	{0, 0, 0}, {1, 0, 2}, {2, 1, 4}, {3, 1, 6},
    {-1, 0, 0}};

	static vi_chn_map_t hisi3521a_vi_chn_map_v1_x17[] = {
       {4, 2, 8}, {5, 2, 9}, {6, 3, 12}, {7, 3, 13},
	{0, 0, 0}, {1, 0, 1}, {2, 1, 4}, {3, 1, 5},
//	{4, 2, 8}, {5, 2, 10}, {6, 3, 12}, {7, 3, 14},
//	{0, 0, 0}, {1, 0, 2}, {2, 1, 4}, {3, 1, 6},	
    {-1, 0, 0}};
	
	static vi_chn_map_t d5_3521a_vi_chn_map_v1_sd[] = {
       {4, 1, 4}, {5, 1, 5}, {6, 1, 5}, {7, 1, 7},
	{0, 0, 0}, {1, 0, 1}, {2, 1, 2}, {3, 1, 3},
//	{0, 0, 2}, {1, 0, 0}, {2, 1, 6}, {3, 1, 4},
    {-1, 0, 0}};
#if 1
    static vi_chn_map_t hisi3520d_vi_chn_map_v1_sd[] = {
    {4, 1, 4}, {5, 1, 5}, {6, 1, 6}, {7, 1, 7},
    {0, 1, 4}, {1, 1, 6}, {2, 0, 0}, {3, 0, 2},
    {-1, 0, 0}};
#endif
    static vi_chn_map_t hisi3520d_vi_chn_map_v1_normal[] = {
    {4, 1, 4}, {5, 1, 5}, {6, 1, 6}, {7, 1, 7},
    {0, 0, 0}, {1, 0, 1}, {2, 0, 2}, {3, 0, 3}, 
    {-1, 0, 0}};
    
	static vi_chn_map_t hisi3521a_vi_chn_map_v1_6A[] = {
		{4, 2, 8}, {5, 2, 10}, {6, 3, 12}, {7, 3, 14},
		{0, 0, 0}, {1, 0, 2}, {2, 1, 4}, {3, 1, 6}, 
		{-1, 0, 0}};
	static vi_chn_map_t hisi3521a_vi_chn_map_v1_P1[] = {	//注:P1机型其实没有vi输入，但为了纯音频通道，此处使用虚假的通道映射///
		{0, 0, 0}, {1, 0, 1}, {2, 0, 2}, {3, 0, 3},
		{4, 1, 4}, {5, 1, 5}, {6, 1, 6}, {7, 1, 7},
		/*{8, 3, 8}, {9, 3, 9},*/
		{-1, 0, 0}};
	static vi_chn_map_t hisi3521a_vi_chn_map_v1_m0026[] = {
		{0, 0, 0}, {1, 0, 2}, {2, 1, 4}, {3, 1, 6},
		{-1, 0, 0}};
    
    if(((product_type == PTD_X3) && (subproduct_type == SYSTEM_TYPE_X3_AHD))
        ||((product_type == PTD_X1_AHD) && ((subproduct_type == SYSTEM_TYPE_X1_V2_X1H0401_AHD)
        ||(subproduct_type == SYSTEM_TYPE_X1_V2_M1H0401_AHD)
        ||(subproduct_type == SYSTEM_TYPE_X1_V2_M1SH0401_AHD)
        ||(subproduct_type == SYSTEM_TYPE_X1_V2_X1N0400_AHD)))
        ||((product_type == PTD_D5) && (subproduct_type == SYSTEM_TYPE_D5_AHD)))
    {
//		printf("hisi3520d_vi_chn_map_v1_sd...hswang\n");
		hisi3520d_vi_chn_map_v1 = hisi3520d_vi_chn_map_v1_sd;
	}
	else if((product_type == PTD_XMD3104)||(product_type == PTD_ES4206))
	{
		hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_sd;
	}
	else if(product_type == PTD_ES8412)
	{
		//printf("subproduct_type:%d\n",subproduct_type);
		if(subproduct_type == SYSTEM_TYPE_ES8412_V1)
		{
			hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_x17;
		}
		else if(subproduct_type == SYSTEM_TYPE_ES8412_V2)
		{
			hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_6A;
		}
	}	
	else if(product_type == PTD_D5_35)
	{
		hisi3520d_vi_chn_map_v1 = d5_3521a_vi_chn_map_v1_sd;
	}	
	else if(product_type == PTD_6A_II_AVX)
	{
		hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_6A;
	}
	else if(product_type == PTD_A5_AHD)
	{
		hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_6A;
	}
	else if(product_type == PTD_P1)
	{
		hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_P1;
	}
	else if ((product_type == PTD_M0026)||(product_type == PTD_M3))
	{
		hisi3520d_vi_chn_map_v1 = hisi3521a_vi_chn_map_v1_m0026;
	}
	else
	{
		hisi3520d_vi_chn_map_v1 = hisi3520d_vi_chn_map_v1_normal;
	}
    pVi_chn_map = hisi3520d_vi_chn_map_v1;	
#elif defined(_AV_PLATFORM_HI3518C_)
    static vi_chn_map_t hisi3518c_vi_chn_map_vi[] = { {0, 0, 0 }, {-1, -1, -1}};
    
    pVi_chn_map = hisi3518c_vi_chn_map_vi;
#else
    _AV_FATAL_(1, "you must select a platform!\n");
#endif
    
    while(pVi_chn_map[index].phy_chn_num != -1)
    {
        if(pVi_chn_map[index].phy_chn_num == phy_chn_num)
        {
            _AV_POINTER_ASSIGNMENT_(pVi_dev, pVi_chn_map[index].vi_dev);
            _AV_POINTER_ASSIGNMENT_(pVi_chn, pVi_chn_map[index].primary_vi_chn);
            return 0;
        }
        index ++;
    }

    return -1;
}

int CHiVi::HiVi_get_primary_vi_chn_config(int phy_chn_num, vi_config_set_t *pPrimary_config, vi_config_set_t *pMinor_config/*=NULL*/, vi_config_set_t *pSource_config/*=NULL*/)
{
    std::map<int, vi_config_set_t>::iterator primary_config_it = m_primary_channel_primary_attr.find(phy_chn_num);
    std::map<int, vi_config_set_t>::iterator minor_config_it = m_primary_channel_minor_attr.find(phy_chn_num);
    std::map<int, vi_config_set_t>::iterator source_config_it = m_source_config.find(phy_chn_num);

    if (primary_config_it != m_primary_channel_primary_attr.end())
    {
        if (pPrimary_config != NULL)
        {
            memset(pPrimary_config, 0, sizeof(vi_config_set_t));
            memcpy(pPrimary_config, &(primary_config_it->second), sizeof(vi_config_set_t));
        }
    }

    if (minor_config_it != m_primary_channel_minor_attr.end())
    {
        if (pMinor_config != NULL)
        {
            memset(pMinor_config, 0, sizeof(vi_config_set_t));
            memcpy(pMinor_config, &(minor_config_it->second), sizeof(vi_config_set_t));
        }
    }
    
    if (source_config_it != m_source_config.end())
    {
        if (pSource_config != NULL)
        {
            memset(pSource_config, 0, sizeof(vi_config_set_t));
            memcpy(pSource_config, &(source_config_it->second), sizeof(vi_config_set_t));
        }
    }

    return 0;
}


int CHiVi::HiVi_get_vi_max_size(int phy_chn_num, int *pMax_width, int *pMax_height, av_resolution_t *pResolution)
{
    /*It's the size of source*/
    int ret_val = -1;

    m_pThread_lock->lock();

#ifndef _AV_PRODUCT_CLASS_IPC_
    std::map<int, vi_config_set_t>::iterator vi_config_it = m_source_config.find(phy_chn_num);

    if(vi_config_it == m_source_config.end())
    {
        DEBUG_ERROR( "CHiVi::HiVi_get_vi_max_size I DON'T KNOW(%d)\n", phy_chn_num);
        goto _OVER_;
    }
    _AV_POINTER_ASSIGNMENT_(pResolution, m_source_config[phy_chn_num].resolution);

    ret_val = HiVi_get_video_size(&m_source_config[phy_chn_num], pMax_width, pMax_height);
#else
    VI_CHN vi_chn = 0;
    HI_S32 s32Result = -1;
    if(this->HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn  HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
        goto _OVER_;
    }
    
    VI_CHN_ATTR_S stuChnAttr;
    memset( &stuChnAttr, 0, sizeof(stuChnAttr) );
    
    s32Result = HI_MPI_VI_GetChnAttr( vi_chn, &stuChnAttr ); 
    if( s32Result != HI_SUCCESS || stuChnAttr.stDestSize.u32Width == 0 || stuChnAttr.stDestSize.u32Height == 0 )
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn  HI_MPI_VI_GetChnAttr FAILED with(%d) vichn=%d phychn=%d\n", s32Result, vi_chn, phy_chn_num);
        goto _OVER_;
    }
    (*pMax_width) = stuChnAttr.stDestSize.u32Width;
    (*pMax_height) = stuChnAttr.stDestSize.u32Height;
    
    ret_val = 0;
#endif

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}


int CHiVi::HiVi_preview_control(int phy_chn_num, bool enter_preview)
{
    std::map<int, vi_config_set_t>::iterator config_it;
    int vi_chn = 0;
    VI_CHN_ATTR_S vi_chn_attr;
    int ret_val = -1;

    DEBUG_MESSAGE( "CHiVi::HiVi_preview_control(channel:%d)(enter:%d)\n", phy_chn_num, enter_preview);

    m_pThread_lock->lock();

    config_it = m_source_config.find(phy_chn_num);
    if(config_it == m_source_config.end())
    {
        DEBUG_ERROR( "CHiVi::HiVi_preview_control WARNING NO SOURCE INFO(%d)\n", phy_chn_num);
        goto _OVER_;
    }

    config_it = m_primary_channel_primary_attr.find(phy_chn_num);
    if(config_it == m_primary_channel_primary_attr.end())
    {
        DEBUG_ERROR( "CHiVi::HiVi_preview_control WARNING NO PRIMARY INFO(%d)\n", phy_chn_num);
        goto _OVER_;
    }

    if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_preview_control HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
        goto _OVER_;
    }

    if((ret_val = HI_MPI_VI_GetChnAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_preview_control HI_MPI_VI_GetChnAttr FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
        ret_val = -1;
        goto _OVER_;
    }

    if(enter_preview == true)
    {
        /*same as vi source*/
        if((m_source_config[phy_chn_num].resolution != m_primary_channel_primary_attr[phy_chn_num].resolution) || 
                        (m_source_config[phy_chn_num].frame_rate != m_primary_channel_primary_attr[phy_chn_num].frame_rate))
        {
            config_it = m_primary_channel_minor_attr.find(phy_chn_num);
            if(config_it != m_primary_channel_minor_attr.end())
            {
                if(!((m_source_config[phy_chn_num].resolution != m_primary_channel_minor_attr[phy_chn_num].resolution) || 
                                (m_source_config[phy_chn_num].frame_rate != m_primary_channel_minor_attr[phy_chn_num].frame_rate)))
                {
                    ret_val = 0;
                    goto _OVER_;
                }
            }

            m_preview_counter[phy_chn_num] ++;
            if(m_preview_counter[phy_chn_num] > 1)
            {
                ret_val = 0;/*It have already been set*/
                goto _OVER_;
            }

            /*primary attribute*/
            if(m_source_config[phy_chn_num].resolution != m_primary_channel_primary_attr[phy_chn_num].resolution)
            {
                HiVi_get_video_size(&m_source_config[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
                switch(m_source_config[phy_chn_num].resolution)
                {
                    case _AR_HD1_:
                    case _AR_CIF_:
                    case _AR_QCIF_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                        break;

                    default:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                        break;
                }
                if((ret_val = HI_MPI_VI_SetChnAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
                {
                    DEBUG_ERROR( "CHiVi::HiVi_preview_control HI_MPI_VI_SetChnAttr FAILED(vi_chn:%d)(0x%08lx)(w:%d, h:%d)\n", vi_chn, (unsigned long)ret_val, vi_chn_attr.stDestSize.u32Width, vi_chn_attr.stDestSize.u32Height);
                    ret_val = -1;
                    goto _OVER_;
                }
            }

            /*minor attribute*/
            if(config_it != m_primary_channel_minor_attr.end())
            {
                HiVi_get_video_size(&m_source_config[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
                switch(m_source_config[phy_chn_num].resolution)
                {
                    case _AR_HD1_:                    
                    case _AR_CIF_:
                    case _AR_QCIF_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                        break;

                    default:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                        break;
                }
                if((ret_val = HI_MPI_VI_SetChnMinorAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
                {
                    DEBUG_ERROR( "CHiVi::HiVi_preview_control HI_MPI_VI_SetChnMinorAttr FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
                    ret_val = -1;
                    goto _OVER_;
                }
            }            
        }
        else
        {
            ret_val = 0;
            goto _OVER_;
        }
    }
    else
    {
        /*restore*/
        if((m_source_config[phy_chn_num].resolution != m_primary_channel_primary_attr[phy_chn_num].resolution) || 
                        (m_source_config[phy_chn_num].frame_rate != m_primary_channel_primary_attr[phy_chn_num].frame_rate))
        {
            config_it = m_primary_channel_minor_attr.find(phy_chn_num);
            if(config_it != m_primary_channel_minor_attr.end())
            {
                if(!((m_source_config[phy_chn_num].resolution != m_primary_channel_minor_attr[phy_chn_num].resolution) || 
                                (m_source_config[phy_chn_num].frame_rate != m_primary_channel_minor_attr[phy_chn_num].frame_rate)))
                {
                    ret_val = 0;
                    goto _OVER_;
                }
            }
            
            if(m_preview_counter[phy_chn_num] > 1)
            {
                /*also use it*/
                m_preview_counter[phy_chn_num] --;
                ret_val = 0;
                goto _OVER_;
            }
            m_preview_counter[phy_chn_num] = 0;

            /*primary attribute*/
            if(m_source_config[phy_chn_num].resolution != m_primary_channel_primary_attr[phy_chn_num].resolution)
            {
                HiVi_get_video_size(&m_primary_channel_primary_attr[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
                switch(m_primary_channel_primary_attr[phy_chn_num].resolution)
                {
                    case _AR_CIF_:
                    case _AR_QCIF_:
                    case _AR_HD1_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:                        
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                        break;

                    default:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                        break;
                }
                if((ret_val = HI_MPI_VI_SetChnAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
                {
                    DEBUG_ERROR( "CHiVi::HiVi_preview_control HI_MPI_VI_SetChnAttr FAILED(0x%08lx)\n", (unsigned long)ret_val);
                    ret_val = -1;
                    goto _OVER_;
                }
            }

            /*minor attribute*/
            if(config_it != m_primary_channel_minor_attr.end())
            {
                HiVi_get_video_size(&m_primary_channel_minor_attr[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
                switch(m_primary_channel_minor_attr[phy_chn_num].resolution)
                {
                    case _AR_CIF_:
                    case _AR_QCIF_:
                    case _AR_HD1_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:                        
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                        break;

                    default:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                        break;
                }
                if((ret_val = HI_MPI_VI_SetChnMinorAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
                {
                    DEBUG_ERROR( "CHiVi::HiVi_preview_control HI_MPI_VI_SetChnMinorAttr FAILED(vi_chn:%d)(0x%08lx)\n", vi_chn, (unsigned long)ret_val);
                    ret_val = -1;
                    goto _OVER_;
                }
            }
        }
        else
        {
            ret_val = 0;
            goto _OVER_;
        }
    }

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}


int CHiVi::HiVi_vi_source_default_config(av_tvsystem_t tv_system, int phy_chn_num, vi_config_set_t *pVi_source_config)
{
    if(HiVi_get_primary_vi_info(phy_chn_num, NULL, NULL, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_source_default_config  HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
        return -1;
    }
    
    /*give the default config of source */
    pVi_source_config->resolution = _AR_D1_;
    pVi_source_config->frame_rate = (tv_system == _AT_PAL_) ? 25 : 30;
#if defined(_AV_PRODUCT_CLASS_IPC_)
    switch(pgAvDeviceInfo->Adi_product_type())
    {
        case PTD_712_VD:
        case PTD_712_VA:
        case PTD_712_VB:
        case PTD_712_VC:
        case PTD_714_VA:
        case PTD_712_VE:
        case PTD_712_VF:
        case PTD_714_VB:
        default:
        {
           pVi_source_config->resolution = _AR_720_;//_AR_960P_;//;
           pVi_source_config->progress = true;
           pVi_source_config->frame_rate =  (tv_system == _AT_PAL_)?25:30;
           pVi_source_config->u8Rotate = 0;
        }
            break;
        case PTD_913_VA:
        {
            pVi_source_config->resolution = _AR_1080_;
            pVi_source_config->progress = true;
            pVi_source_config->frame_rate =  (tv_system == _AT_PAL_)?25:30;
            pVi_source_config->u8Rotate = 0; 
        }
            break;
            
    }
#endif
    return 0;
}


int CHiVi::HiVi_start_all_minor_vi_chn(vi_config_set_t *pPrimary_attr/* = NULL*/, vi_config_set_t *pMinor_attr/* = NULL*/)
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_max_channel_number();
    VI_CHN_ATTR_S vi_chn_attr;
    int phy_chn_num = 0;
    vi_config_set_t primary_config;
    int vi_chn = 0;
    HI_S32 ret_val = -1;

    m_pThread_lock->lock();

    if(m_source_config.size() == 0)/*same as primary channel*/
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_all_minor_vi_chn  FAILED (m_source_config.size() != 0)\n");
        ret_val = -1;
        goto _OVER_;
    }

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        
        if (_VS_DIGITAL_ == pgAvDeviceInfo->Adi_get_video_source(phy_chn_num))
        {
            DEBUG_WARNING( "CHiVi::HiVi_start_all_minor_vi_chn(...) chn %d is digital!!! \n", phy_chn_num);
            continue;
        }
        
        if(HiVi_get_minor_vi_info(phy_chn_num, NULL, &vi_chn) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_start_all_minor_vi_chn  HiVi_get_minor_vi_info FAILED(%d)\n", phy_chn_num);
            ret_val = -1;
            goto _OVER_;
        }

        memset(&vi_chn_attr, 0, sizeof(VI_CHN_ATTR_S));

        /*primary attribute*/
        if(pPrimary_attr == NULL)
        {
            primary_config.resolution = _AR_D1_;
            primary_config.frame_rate = (m_tv_system == _AT_PAL_)?25:30;
        }
        else
        {
            memcpy(&primary_config, pPrimary_attr + phy_chn_num, sizeof(vi_config_set_t));
        }

        /*capture rectangle*/
        switch(m_source_config[phy_chn_num].resolution)
        {
            case _AR_D1_:
                vi_chn_attr.stCapRect.s32X = (720 - pgAvDeviceInfo->Adi_get_D1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;

            case _AR_960H_WD1_:
                vi_chn_attr.stCapRect.s32X = (960 - pgAvDeviceInfo->Adi_get_WD1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;

            default:
                vi_chn_attr.stCapRect.s32X = 0;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;
        }
        HiVi_get_video_size(&m_source_config[phy_chn_num], (int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);

        /*destination size*/
        switch(primary_config.resolution)
        {
            case _AR_HD1_:
            case _AR_CIF_:
            case _AR_QCIF_:
            case _AR_960H_WHD1_:
            case _AR_960H_WCIF_:
            case _AR_960H_WQCIF_:
                vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                break;

            default:
                vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                break;
        }
        HiVi_get_video_size(&primary_config, (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);

        /*other parameter*/
        vi_chn_attr.enPixFormat = pgAvDeviceInfo->Adi_get_vi_pixel_format();
        
        vi_chn_attr.bMirror = (primary_config.u8Mirror==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
        vi_chn_attr.bFlip = (primary_config.u8Flip ==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
#ifndef _AV_PLATFORM_HI3520D_V300_
        vi_chn_attr.bChromaResample = HI_FALSE;

        /*frame rate*/
        if(pPrimary_attr == NULL)
        {
            vi_chn_attr.s32SrcFrameRate = -1;
            vi_chn_attr.s32FrameRate = -1;
        }
        else
        {
            vi_chn_attr.s32SrcFrameRate = m_source_config[phy_chn_num].frame_rate;
            vi_chn_attr.s32FrameRate = pPrimary_attr[phy_chn_num].frame_rate;
        }
 #else
        /*frame rate*/
        if(pPrimary_attr == NULL)
        {
            vi_chn_attr.s32SrcFrameRate = -1;
            vi_chn_attr.s32DstFrameRate = -1;
        }
        else
        {
            vi_chn_attr.s32SrcFrameRate = m_source_config[phy_chn_num].frame_rate;
            vi_chn_attr.s32DstFrameRate = pPrimary_attr[phy_chn_num].frame_rate;
        }
 #endif
 #ifndef _AV_PRODUCT_CLASS_IPC_
        if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num) == 1)
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif
            vi_chn_attr.stCapRect.u32Width=1280;
            vi_chn_attr.stCapRect.u32Height=720;
            vi_chn_attr.stDestSize.u32Width=1280;
            vi_chn_attr.stDestSize.u32Height=720;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
            DEBUG_ERROR("phy_chn_num[%d],vi_chn[%d],stCapRect(w,h)=[%d,%d],enPixFormat[%d]\n",
            phy_chn_num,vi_chn,vi_chn_attr.stCapRect.u32Width,vi_chn_attr.stCapRect.u32Height,vi_chn_attr.enPixFormat);				 

        }
		else if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num) == 2)
		{
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif
            vi_chn_attr.stCapRect.u32Width=1920;
            vi_chn_attr.stCapRect.u32Height=1080;
            vi_chn_attr.stDestSize.u32Width=1920;
            vi_chn_attr.stDestSize.u32Height=1080;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
		}
        else
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_INTERLACED;
#endif
            if(pgAvDeviceInfo->Adi_ad_type_isahd()==true)
            {
                vi_chn_attr.stCapRect.u32Width=928;
                vi_chn_attr.stCapRect.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
                vi_chn_attr.stDestSize.u32Width=928;
                vi_chn_attr.stDestSize.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
            }
        }
#endif
        /*primary attribute*/
#ifdef _AV_PLATFORM_HI3518C_
        if(HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr, primary_config.u8Rotate) != 0)
#else
        if(HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr) != 0)
#endif
        {
            DEBUG_ERROR( "CHiVi::HiVi_start_all_minor_vi_chn HiVi_start_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
            ret_val = -1;
            goto _OVER_;
        }

        /*minor attribute*/
        if(pMinor_attr != NULL)
        {
            switch(pMinor_attr[phy_chn_num].resolution)
            {
                case _AR_HD1_:
                case _AR_CIF_:
                case _AR_QCIF_:
                case _AR_960H_WHD1_:
                case _AR_960H_WCIF_:
                case _AR_960H_WQCIF_:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                    break;

                default:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                    break;
            }
            HiVi_get_video_size(&pMinor_attr[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);

            if((ret_val = HI_MPI_VI_SetChnMinorAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
            {
                DEBUG_ERROR( "CHiVi::HiVi_start_all_minor_vi_chn HI_MPI_VI_SetChnMinorAttr FAILED(0x%08lx)\n", (unsigned long)ret_val);
                ret_val = -1;
                goto _OVER_;
            }

            memcpy(&m_minor_channel_minor_attr[phy_chn_num], pMinor_attr + phy_chn_num, sizeof(vi_config_set_t));
        }

        memcpy(&m_minor_channel_primary_attr[phy_chn_num], &primary_config, sizeof(vi_config_set_t));
        m_preview_counter[phy_chn_num] = 0;
    }

    ret_val = 0;

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}


int CHiVi::HiVi_stop_all_minor_vi_chn()
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_max_channel_number();
    int phy_chn_num = 0;
    int vi_chn = 0;
    int ret_val = -1;

    m_pThread_lock->lock();

    if(m_minor_channel_primary_attr.size() == 0)
    {
        DEBUG_WARNING( "CHiVi::HiVi_stop_all_minor_vi_chn WARNING (m_minor_channel_primary_attr.size() == 0)\n");
        ret_val = 0;
        goto _OVER_;
    }

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
        {
            DEBUG_WARNING( "CHiVi::HiVi_stop_all_minor_vi_chn(...) chn %d is digital!!! \n", phy_chn_num);
            continue;
        }
        
        if(HiVi_get_minor_vi_info(phy_chn_num, NULL, &vi_chn) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_stop_all_minor_vi_chn HiVi_get_minor_vi_info FAILED(%d)\n", phy_chn_num);
            goto _OVER_;
        }

        if(HiVi_stop_vi_chn(vi_chn) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_stop_all_minor_vi_chn HiVi_stop_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
            goto _OVER_;
        }
        std::map<int, vi_config_set_t>::iterator phy_chn_it;

        phy_chn_it = m_minor_channel_primary_attr.find(phy_chn_num);
        if (phy_chn_it != m_minor_channel_primary_attr.end())
        {
            m_minor_channel_primary_attr.erase(phy_chn_num);
        }

        phy_chn_it = m_minor_channel_minor_attr.find(phy_chn_num);
        if (phy_chn_it != m_minor_channel_minor_attr.end())
        {
            m_minor_channel_minor_attr.erase(phy_chn_num);
        }
        
    }

    ret_val = 0;

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}

int CHiVi::HiVi_start_all_primary_vi_chn(av_tvsystem_t tv_system, vi_config_set_t *pSource/* = NULL*/, vi_config_set_t *pPrimary_attr/* = NULL*/, vi_config_set_t *pMinor_attr/* = NULL*/)
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_get_vi_chn_num();
    VI_CHN_ATTR_S vi_chn_attr;
    int phy_chn_num = 0;
    vi_config_set_t source_config;
    vi_config_set_t primary_config;
    int vi_chn = 0;
    HI_S32 ret_val = -1;
    
    m_pThread_lock->lock();

    if(m_source_config.size() != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn  FAILED (m_source_config.size() != 0)\n");
        ret_val = -1;
        goto _OVER_;
    }

    m_tv_system = tv_system;

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
        {
            DEBUG_WARNING( "CHiVi::HiVi_start_all_primary_vi_chn(...) chn %d is digital!!! \n", phy_chn_num);
            continue;
        }
        
        if (HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn  HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
            ret_val = -1;
            goto _OVER_;
        }

        memset(&vi_chn_attr, 0, sizeof(VI_CHN_ATTR_S));

        /*source config*/
        if (pSource == NULL)
        {
            HiVi_vi_source_default_config(tv_system, phy_chn_num, &source_config);
        }
        else
        {
            memcpy(&source_config, pSource + phy_chn_num, sizeof(vi_config_set_t));
        }

        /*primary attribute*/
        if (pPrimary_attr == NULL)
        {
            switch(source_config.resolution)
            {
                case _AR_D1_:
                case _AR_HD1_:
                case _AR_CIF_:
                case _AR_QCIF_:
                    source_config.resolution = _AR_D1_;
                    break;
                case _AR_960H_WD1_:
                case _AR_960H_WHD1_:
                case _AR_960H_WCIF_:
                case _AR_960H_WQCIF_:
                    source_config.resolution = _AR_960H_WD1_;
                    break;
		case _AR_720_:
			source_config.resolution = _AR_720_;
			break;
                default:
                    break;
            }
            memcpy(&primary_config, &source_config, sizeof(vi_config_set_t));
        }
        else
        {
            memcpy(&primary_config, pPrimary_attr + phy_chn_num, sizeof(vi_config_set_t));
        }

#if defined(_AV_PRODUCT_CLASS_IPC_)
                sensor_e st  = ST_SENSOR_DEFAULT;
                unsigned char chn_id = 0;
                vi_capture_rect_s rect;

                memset(&rect, 0, sizeof(vi_capture_rect_s));
                if(PTD_712_VD == pgAvDeviceInfo->Adi_product_type())
                {
                    if(0 != pgAvDeviceInfo->Adi_get_ipc_work_mode())
                    {
                        chn_id = 1;
                    }
                }
                st = pgAvDeviceInfo->Adi_get_sensor_type();
                pgAvDeviceInfo->Adi_get_vi_config(st, chn_id, pgAvDeviceInfo->Adi_get_vi_max_resolution(), rect);
                
                vi_chn_attr.stCapRect.s32X = rect.m_vi_capture_x;

                if((PTD_712_VD == pgAvDeviceInfo->Adi_product_type()) && (0 != pgAvDeviceInfo->Adi_get_ipc_work_mode()))
                {
                    int y = rect.m_vi_capture_y + pgAvDeviceInfo->Adi_get_vi_chn_capture_y_offset();
                    if(y<0)
                    {
                        y = 0;
                    }
                    int width = 0;
                    int height = 0;
                    HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(), &width, &height);
                    if(y > rect.m_vi_capture_height - height)
                    {
                        y = rect.m_vi_capture_height - height;
                    }
                    
                    vi_chn_attr.stCapRect.s32Y = y;
                }
                else
                {
                    vi_chn_attr.stCapRect.s32Y = rect.m_vi_capture_y;  
                }

                 vi_chn_attr.stCapRect.s32X =  (vi_chn_attr.stCapRect.s32X + 4 -1)/4*4;
                 vi_chn_attr.stCapRect.s32Y = (vi_chn_attr.stCapRect.s32Y + 4 -1)/4*4;
                
#else            
        /*capture rectangle*/
        switch(source_config.resolution)
        {
            case _AR_D1_:
            case _AR_HD1_:
            case _AR_CIF_:
            case _AR_QCIF_:
                vi_chn_attr.stCapRect.s32X = (720 - pgAvDeviceInfo->Adi_get_D1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;
            case _AR_960H_WD1_:
            case _AR_960H_WHD1_:
            case _AR_960H_WCIF_:
            case _AR_960H_WQCIF_:
                vi_chn_attr.stCapRect.s32X = (960 - pgAvDeviceInfo->Adi_get_WD1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;
            default:
            vi_chn_attr.stCapRect.s32X = 0;
            vi_chn_attr.stCapRect.s32Y = 0;
                break;
        }
#endif

#if defined(_AV_PRODUCT_CLASS_IPC_)
        HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(),(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
#else
        HiVi_get_video_size(&source_config, (int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
#endif
        // DEBUG_MESSAGE("#####phy_chn_num = %d vi_chn_attr( w = %d h = %d )\n", vi_chn_attr.stCapRect.u32Width, vi_chn_attr.stCapRect.u32Height);
        
        /*destination size*/        
        switch(primary_config.resolution)
        {
            case _AR_HD1_:
            case _AR_CIF_:
            case _AR_QCIF_:
            case _AR_960H_WHD1_:
            case _AR_960H_WCIF_:
            case _AR_960H_WQCIF_:
                vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                break;
            default:
                vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                break;
        }

#if defined(_AV_PRODUCT_CLASS_IPC_)
        HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(), (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
#else
        HiVi_get_video_size(&primary_config, (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
#endif
        
        pgAvDeviceInfo->Adi_set_cur_chn_res(phy_chn_num, vi_chn_attr.stDestSize.u32Width, vi_chn_attr.stDestSize.u32Height);
        
        /*other parameter*/
        vi_chn_attr.enPixFormat = pgAvDeviceInfo->Adi_get_vi_pixel_format();
        vi_chn_attr.bMirror = (source_config.u8Mirror==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
        vi_chn_attr.bFlip = (source_config.u8Flip ==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
        
#ifndef  _AV_PLATFORM_HI3520D_V300_
        vi_chn_attr.bChromaResample = HI_FALSE;

        /*frame rate*/
        if (pPrimary_attr != NULL)
        {
            vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
            vi_chn_attr.s32FrameRate = pPrimary_attr[phy_chn_num].frame_rate;
        }
        else if ((source_config.frame_rate > 30) && (source_config.resolution == _AR_1080_))
        {
            vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
            vi_chn_attr.s32FrameRate = 30;
            primary_config.frame_rate = 30;
        }
        else
        {
            vi_chn_attr.s32SrcFrameRate = -1;
            vi_chn_attr.s32FrameRate = -1;
        }
#else
        /*frame rate*/
        if (pPrimary_attr != NULL)
        {
            vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
            vi_chn_attr.s32DstFrameRate = pPrimary_attr[phy_chn_num].frame_rate;
        }
        else if ((source_config.frame_rate > 30) && (source_config.resolution == _AR_1080_))
        {
            vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
            vi_chn_attr.s32DstFrameRate = 30;
            primary_config.frame_rate = 30;
        }
        else
        {
            vi_chn_attr.s32SrcFrameRate = -1;
            vi_chn_attr.s32DstFrameRate = -1;
        }
#endif
#ifndef _AV_PRODUCT_CLASS_IPC_
		DEBUG_CRITICAL("pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num:%d)...%d\n",phy_chn_num,pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num));
        if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num)== 1)
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif
			if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)	
			{
				pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_720_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
			}
			else
			{
            	vi_chn_attr.stCapRect.u32Width=1280;
            	vi_chn_attr.stCapRect.u32Height=720;
			}
            vi_chn_attr.stDestSize.u32Width=1280;
            vi_chn_attr.stDestSize.u32Height=720;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
            DEBUG_ERROR("phy_chn_num[%d],vi_chn[%d],stCapRect(w,h)=[%d,%d],enPixFormat[%d]\n",
            phy_chn_num,vi_chn,vi_chn_attr.stCapRect.u32Width,vi_chn_attr.stCapRect.u32Height,vi_chn_attr.enPixFormat); 

        }
		else if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num)== 2)
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif
            vi_chn_attr.stCapRect.u32Width=1920;
            vi_chn_attr.stCapRect.u32Height=1080;
            vi_chn_attr.stDestSize.u32Width=1920;
            vi_chn_attr.stDestSize.u32Height=1080;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
            DEBUG_ERROR("phy_chn_num[%d],vi_chn[%d],stCapRect(w,h)=[%d,%d],enPixFormat[%d]\n",
            phy_chn_num,vi_chn,vi_chn_attr.stCapRect.u32Width,vi_chn_attr.stCapRect.u32Height,vi_chn_attr.enPixFormat); 

        }
        else
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_INTERLACED;
#endif
            if(pgAvDeviceInfo->Adi_ad_type_isahd()==true)
            {
            	if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)	
				{
					pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_960H_WD1_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
					vi_chn_attr.stCapRect.s32X = 64;
				}
				else
				{
					vi_chn_attr.stCapRect.u32Width=928;
                	vi_chn_attr.stCapRect.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
				}
				
                vi_chn_attr.stDestSize.u32Width=928;
                vi_chn_attr.stDestSize.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
            }

                        //! for REI, demand videoloss record
#if defined(_AV_PRODUCT_CLASS_DVR_REI_)
            DEBUG_CRITICAL("primary chn:%d, w:%d, h:%d\n", phy_chn_num, source_config.w, source_config.h);
            if (  source_config.w == 1280)
            {
                vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;

    			if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)
    			{
    				pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_720_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
    			}
    			else
    			{
    				vi_chn_attr.stCapRect.u32Width=1280;
    				vi_chn_attr.stCapRect.u32Height=720;
    			}
                vi_chn_attr.stDestSize.u32Width=1280;
                vi_chn_attr.stDestSize.u32Height=720;
                vi_chn_attr.stCapRect.s32X = 0;                
            }
            else if (  source_config.w == 1920)
            {

                vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;

                vi_chn_attr.stCapRect.u32Width=1920;
                vi_chn_attr.stCapRect.u32Height=1080;
                vi_chn_attr.stDestSize.u32Width=1920;
                vi_chn_attr.stDestSize.u32Height=1080;
                vi_chn_attr.stCapRect.s32X = 0;
            }
#endif
        }
#endif
        /*primary attribute*/
#ifdef _AV_PLATFORM_HI3518C_
        if (HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr, source_config.u8Rotate) != 0)
#else
        if (HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr) != 0)
#endif
        {
            DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn HiVi_start_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
            ret_val = -1;
            goto _OVER_;
        }

        /*minor attribute*/
        if (pMinor_attr != NULL)
        {
            switch(pMinor_attr[phy_chn_num].resolution)
            {
                case _AR_HD1_:
                case _AR_CIF_:
                case _AR_QCIF_:
                case _AR_960H_WHD1_:
                case _AR_960H_WCIF_:
                case _AR_960H_WQCIF_:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                    break;

                default:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                    break;
            }
            HiVi_get_video_size(&pMinor_attr[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);


            if ((ret_val = HI_MPI_VI_SetChnMinorAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
            {
                DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn HI_MPI_VI_SetChnMinorAttr FAILED(0x%08lx)\n", (unsigned long)ret_val);
                ret_val = -1;
                goto _OVER_;
            }

            memcpy(&m_primary_channel_minor_attr[phy_chn_num], pMinor_attr + phy_chn_num, sizeof(vi_config_set_t));
        }

        memcpy(&m_source_config[phy_chn_num], &source_config, sizeof(vi_config_set_t));
        memcpy(&m_primary_channel_primary_attr[phy_chn_num], &primary_config, sizeof(vi_config_set_t));
        m_preview_counter[phy_chn_num] = 0;
    }
    ret_val = 0;

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}

int CHiVi::HiVi_start_selected_primary_vi_chn(av_tvsystem_t tv_system, unsigned long long int chn_mask, vi_config_set_t *pSource/* = NULL*/, vi_config_set_t *pPrimary_attr/* = NULL*/, vi_config_set_t *pMinor_attr/* = NULL*/)
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_get_vi_chn_num();
    VI_CHN_ATTR_S vi_chn_attr;
    int phy_chn_num = 0;
    vi_config_set_t source_config;
    vi_config_set_t primary_config;
    int vi_chn = 0;
    HI_S32 ret_val = -1;
    
    m_pThread_lock->lock();

    m_tv_system = tv_system;

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        if((chn_mask) & (1LL<<phy_chn_num))
        {
            if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
            {
                DEBUG_WARNING( "CHiVi::HiVi_start_selected_primary_vi_chn(...) chn %d is digital!!! \n", phy_chn_num);
                continue;
            }
            
            if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
            {
                DEBUG_ERROR( "CHiVi::HiVi_start_selected_primary_vi_chn  HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
                ret_val = -1;
                goto _OVER_;
            }

            memset(&vi_chn_attr, 0, sizeof(VI_CHN_ATTR_S));

            /*source config*/
            if(pSource == NULL)
            {
                HiVi_vi_source_default_config(tv_system, phy_chn_num, &source_config);
            }
            else
            {
                memcpy(&source_config, pSource + phy_chn_num, sizeof(vi_config_set_t));
            }

            /*primary attribute*/
            if(pPrimary_attr == NULL)
            {
                switch(source_config.resolution)
                {
                    case _AR_D1_:
                    case _AR_HD1_:
                    case _AR_CIF_:
                    case _AR_QCIF_:
                        source_config.resolution = _AR_D1_;
                        break;
                    case _AR_960H_WD1_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:
                        source_config.resolution = _AR_960H_WD1_;
                        break;
                    default:
                        break;
                }
                memcpy(&primary_config, &source_config, sizeof(vi_config_set_t));
            }
            else
            {
                memcpy(&primary_config, pPrimary_attr + phy_chn_num, sizeof(vi_config_set_t));
            }

#if defined(_AV_PRODUCT_CLASS_IPC_)
                sensor_e st  = ST_SENSOR_DEFAULT;
                unsigned char chn_id = 0;
                vi_capture_rect_s rect;

                memset(&rect, 0, sizeof(vi_capture_rect_s));
                if(PTD_712_VD == pgAvDeviceInfo->Adi_product_type())
                {
                    if(0 != pgAvDeviceInfo->Adi_get_ipc_work_mode())
                    {
                        chn_id = 1;
                    }
                }
                st = pgAvDeviceInfo->Adi_get_sensor_type();
                pgAvDeviceInfo->Adi_get_vi_config(st, chn_id, pgAvDeviceInfo->Adi_get_vi_max_resolution(), rect);
                vi_chn_attr.stCapRect.s32X = rect.m_vi_capture_x;              
                if((PTD_712_VD == pgAvDeviceInfo->Adi_product_type()) && (0 != pgAvDeviceInfo->Adi_get_ipc_work_mode()))
                {
                    int y = rect.m_vi_capture_y + pgAvDeviceInfo->Adi_get_vi_chn_capture_y_offset();
                    if(y<0)
                    {
                        y = 0;
                    }
                    int width = 0;
                    int height = 0;
                    HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(), &width, &height);
                    if(y > rect.m_vi_capture_height - height)
                    {
                        y = rect.m_vi_capture_height - height;
                    }
                    
                    vi_chn_attr.stCapRect.s32Y = y;
                }
                else
                {
                    vi_chn_attr.stCapRect.s32Y = rect.m_vi_capture_y;  
                } 
                 vi_chn_attr.stCapRect.s32X =  (vi_chn_attr.stCapRect.s32X + 4 -1)/4*4;
                 vi_chn_attr.stCapRect.s32Y = (vi_chn_attr.stCapRect.s32Y + 4 -1)/4*4;
#else                    
            /*capture rectangle*/
            switch(source_config.resolution)
            {
            case _AR_D1_:
            case _AR_HD1_:
            case _AR_CIF_:
            case _AR_QCIF_:
                vi_chn_attr.stCapRect.s32X = (720 - pgAvDeviceInfo->Adi_get_D1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;
            case _AR_960H_WD1_:
            case _AR_960H_WHD1_:
            case _AR_960H_WCIF_:
            case _AR_960H_WQCIF_:
                vi_chn_attr.stCapRect.s32X = (960 - pgAvDeviceInfo->Adi_get_WD1_width()) / 2;
                vi_chn_attr.stCapRect.s32Y = 0;
                break;
            default:
            vi_chn_attr.stCapRect.s32X = 0;
            vi_chn_attr.stCapRect.s32Y = 0;
                   break;
            }
#endif 

#if defined(_AV_PRODUCT_CLASS_IPC_)
        HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(),(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
#else
        HiVi_get_video_size(&source_config, (int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
#endif
            // DEBUG_MESSAGE("#####phy_chn_num = %d vi_chn_attr( w = %d h = %d )\n", vi_chn_attr.stCapRect.u32Width, vi_chn_attr.stCapRect.u32Height);
            
            /*destination size*/        
            switch(primary_config.resolution)
            {
                case _AR_HD1_:
                case _AR_CIF_:
                case _AR_QCIF_:
                case _AR_960H_WHD1_:
                case _AR_960H_WCIF_:
                case _AR_960H_WQCIF_:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                    break;
                default:
                    vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                    break;
            }
#if defined(_AV_PRODUCT_CLASS_IPC_)
        HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(), (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
#else
        HiVi_get_video_size(&primary_config, (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);
#endif
            
            pgAvDeviceInfo->Adi_set_cur_chn_res(phy_chn_num, vi_chn_attr.stDestSize.u32Width, vi_chn_attr.stDestSize.u32Height);
            
            /*other parameter*/
            vi_chn_attr.enPixFormat = pgAvDeviceInfo->Adi_get_vi_pixel_format();
            
            vi_chn_attr.bMirror = (source_config.u8Mirror==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
            vi_chn_attr.bFlip = (source_config.u8Flip ==0)? HI_FALSE:HI_TRUE;//HI_FALSE;
#ifndef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.bChromaResample = HI_FALSE;

            /*frame rate*/
            if(pPrimary_attr != NULL)
            {
                vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
                vi_chn_attr.s32FrameRate = pPrimary_attr[phy_chn_num].frame_rate;
            }
            else if ((source_config.frame_rate > 30) && (source_config.resolution == _AR_1080_))
            {
                vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
                vi_chn_attr.s32FrameRate = 30;
                primary_config.frame_rate = 30;
            }
            else
            {
                vi_chn_attr.s32SrcFrameRate = -1;
                vi_chn_attr.s32FrameRate = -1;
            }
#else
            /*frame rate*/
            if(pPrimary_attr != NULL)
            {
                vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
                vi_chn_attr.s32DstFrameRate = pPrimary_attr[phy_chn_num].frame_rate;
            }
            else if ((source_config.frame_rate > 30) && (source_config.resolution == _AR_1080_))
            {
                vi_chn_attr.s32SrcFrameRate = source_config.frame_rate;
                vi_chn_attr.s32DstFrameRate = 30;
                primary_config.frame_rate = 30;
            }
            else
            {
                vi_chn_attr.s32SrcFrameRate = -1;
                vi_chn_attr.s32DstFrameRate = -1;
            }

#endif
#ifndef _AV_PRODUCT_CLASS_IPC_
        if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num)== 1)
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif		
			if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)
			{
				pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_720_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
			}
			else
			{
				vi_chn_attr.stCapRect.u32Width=1280;
				vi_chn_attr.stCapRect.u32Height=720;
			}
            vi_chn_attr.stDestSize.u32Width=1280;
            vi_chn_attr.stDestSize.u32Height=720;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
            DEBUG_ERROR("phy_chn_num[%d],vi_chn[%d],stCapRect(w,h)=[%d,%d],enPixFormat[%d]\n",
            phy_chn_num,vi_chn,vi_chn_attr.stCapRect.u32Width,vi_chn_attr.stCapRect.u32Height,vi_chn_attr.enPixFormat);				 

        }
		else if(pgAvDeviceInfo->Adi_videoformat_isahd(phy_chn_num)== 2)
		{
#ifdef _AV_PLATFORM_HI3520D_V300_
            vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;
#endif
            vi_chn_attr.stCapRect.u32Width=1920;
            vi_chn_attr.stCapRect.u32Height=1080;
            vi_chn_attr.stDestSize.u32Width=1920;
            vi_chn_attr.stDestSize.u32Height=1080;
            vi_chn_attr.stCapRect.s32X = 0;
#ifdef _AV_PLATFORM_HI3520D_
            if((vi_chn==6)||(vi_chn==2))
            {
                vi_chn_attr.enPixFormat = PIXEL_FORMAT_YUV_SEMIPLANAR_422;
            }
#endif
            DEBUG_ERROR("phy_chn_num[%d],vi_chn[%d],stCapRect(w,h)=[%d,%d],enPixFormat[%d]\n",
            phy_chn_num,vi_chn,vi_chn_attr.stCapRect.u32Width,vi_chn_attr.stCapRect.u32Height,vi_chn_attr.enPixFormat);				 

        }
        else
        {
#ifdef _AV_PLATFORM_HI3520D_V300_
           vi_chn_attr.enScanMode = VI_SCAN_INTERLACED;
#endif
            if(pgAvDeviceInfo->Adi_ad_type_isahd()==true)
            {
            	if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)
				{
					pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_960H_WD1_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
					vi_chn_attr.stCapRect.s32X = 64;
				}
				else
				{
					vi_chn_attr.stCapRect.u32Width=928;
					vi_chn_attr.stCapRect.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
				}
                vi_chn_attr.stDestSize.u32Width=928;
                vi_chn_attr.stDestSize.u32Height=(m_tv_system == _AT_PAL_) ? 576 : 480;
            }
            //! for REI, demand videoloss record
#if defined(_AV_PRODUCT_CLASS_DVR_REI_)
            DEBUG_CRITICAL("chn:%d, w:%d, h:%d\n", phy_chn_num, source_config.w, source_config.h);
            if (  source_config.w == 1280)
            {
                vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;

    			if(pgAvDeviceInfo->Adi_get_camera_insert_mode() == 1)
    			{
    				pgAvDeviceInfo->Adi_get_video_size_input_mix(phy_chn_num,_AR_720_,m_tv_system,(int *)&vi_chn_attr.stCapRect.u32Width, (int *)&vi_chn_attr.stCapRect.u32Height);
    			}
    			else
    			{
    				vi_chn_attr.stCapRect.u32Width=1280;
    				vi_chn_attr.stCapRect.u32Height=720;
    			}
                vi_chn_attr.stDestSize.u32Width=1280;
                vi_chn_attr.stDestSize.u32Height=720;
                vi_chn_attr.stCapRect.s32X = 0;                
            }
            else if (  source_config.w == 1920)
            {

                vi_chn_attr.enScanMode = VI_SCAN_PROGRESSIVE;

                vi_chn_attr.stCapRect.u32Width=1920;
                vi_chn_attr.stCapRect.u32Height=1080;
                vi_chn_attr.stDestSize.u32Width=1920;
                vi_chn_attr.stDestSize.u32Height=1080;
                vi_chn_attr.stCapRect.s32X = 0;
            }
#endif
            
        }
#endif
            /*primary attribute*/
#ifdef _AV_PLATFORM_HI3518C_
            if(HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr, source_config.u8Rotate) != 0)
#else
            if(HiVi_start_vi_chn(phy_chn_num, vi_chn, &vi_chn_attr) != 0)
#endif
            {
                DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn HiVi_start_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
                ret_val = -1;
                goto _OVER_;
            }

            /*minor attribute*/
            if(pMinor_attr != NULL)
            {
                switch(pMinor_attr[phy_chn_num].resolution)
                {
                    case _AR_HD1_:
                    case _AR_CIF_:
                    case _AR_QCIF_:
                    case _AR_960H_WHD1_:
                    case _AR_960H_WCIF_:
                    case _AR_960H_WQCIF_:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTTOM;
                        break;

                    default:
                        vi_chn_attr.enCapSel = VI_CAPSEL_BOTH;
                        break;
                }
                HiVi_get_video_size(&pMinor_attr[phy_chn_num], (int *)&vi_chn_attr.stDestSize.u32Width, (int *)&vi_chn_attr.stDestSize.u32Height);


                if((ret_val = HI_MPI_VI_SetChnMinorAttr(vi_chn, &vi_chn_attr)) != HI_SUCCESS)
                {
                    DEBUG_ERROR( "CHiVi::HiVi_start_all_primary_vi_chn HI_MPI_VI_SetChnMinorAttr FAILED(0x%08lx)\n", (unsigned long)ret_val);
                    ret_val = -1;
                    goto _OVER_;
                }

                memcpy(&m_primary_channel_minor_attr[phy_chn_num], pMinor_attr + phy_chn_num, sizeof(vi_config_set_t));
            }

            memcpy(&m_source_config[phy_chn_num], &source_config, sizeof(vi_config_set_t));
            memcpy(&m_primary_channel_primary_attr[phy_chn_num], &primary_config, sizeof(vi_config_set_t));
            m_preview_counter[phy_chn_num] = 0;
        }
    }
    ret_val = 0;

_OVER_:

    m_pThread_lock->unlock();

    return ret_val;
}


int CHiVi::HiVi_stop_all_primary_vi_chn()
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_get_vi_chn_num();
    int phy_chn_num = 0;
    int vi_chn = 0;
    int ret_val = -1;

    m_pThread_lock->lock();

    if(m_source_config.size() == 0)
    {
        DEBUG_WARNING( "CHiVi::HiVi_stop_all_primary_vi_chn WARNING (m_source_config.size() == 0)\n");
        ret_val = 0;
        goto _OVER_;
    }
    DEBUG_WARNING( "CHiVi::HiVi_stop_all_primary_vi_chn (m_source_config.size() == %d)\n", m_source_config.size());

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        if (_VS_DIGITAL_ == pgAvDeviceInfo->Adi_get_video_source(phy_chn_num))
        {
            DEBUG_WARNING( "CHiVi::HiVi_stop_all_primary_vi_chn chn %d is digital!!! \n", phy_chn_num);
            continue;
        }
        
        DEBUG_MESSAGE( "CHiVi::HiVi_stop_all_primary_vi_chn chn %d is analoag!!! \n", phy_chn_num);

        if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_stop_all_primary_vi_chn HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
            goto _OVER_;
        }

        VI_CHN_ATTR_S vi_chn_attr;
        HI_S32 s32Rtn = HI_MPI_VI_GetChnMinorAttr( vi_chn, &vi_chn_attr);

        if( s32Rtn == HI_SUCCESS )
        {
            HI_MPI_VI_ClearChnMinorAttr(vi_chn);
        }

        if(HiVi_stop_vi_chn(vi_chn) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_stop_all_primary_vi_chn HiVi_stop_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
            goto _OVER_;
        }
        std::map<int, vi_config_set_t>::iterator phy_chn_it;

        phy_chn_it = m_source_config.find(phy_chn_num);
        if (phy_chn_it != m_source_config.end())
        {
            m_source_config.erase(phy_chn_num);
        }
        
        phy_chn_it = m_primary_channel_primary_attr.find(phy_chn_num);
        if (phy_chn_it != m_primary_channel_primary_attr.end())
        {
            m_primary_channel_primary_attr.erase(phy_chn_num);
        }
        
        phy_chn_it = m_primary_channel_minor_attr.find(phy_chn_num);
        if (phy_chn_it != m_primary_channel_minor_attr.end())
        {
            m_primary_channel_minor_attr.erase(phy_chn_num);
        }
        std::map<int, int>::iterator preview_iterator;
        preview_iterator = m_preview_counter.find(phy_chn_num);
        if (preview_iterator != m_preview_counter.end())
        {
            m_preview_counter.erase(phy_chn_num);
        }
    }

    ret_val = 0;

_OVER_:

    m_pThread_lock->unlock();
    DEBUG_MESSAGE( "CHiVi::HiVi_stop_all_primary_vi_chn success\n");

    return ret_val;
}

int CHiVi::HiVi_stop_selected_primary_vi_chn(unsigned long long int chn_mask)
{
    int max_vi_chn_num = pgAvDeviceInfo->Adi_get_vi_chn_num();
    int phy_chn_num = 0;
    int vi_chn = 0;
    int ret_val = -1;

    m_pThread_lock->lock();

    if(m_source_config.size() == 0)
    {
        DEBUG_WARNING( "CHiVi::HiVi_stop_selected_primary_vi_chn WARNING (m_source_config.size() == 0)\n");
        ret_val = 0;
        goto _OVER_;
    }
    DEBUG_MESSAGE( "CHiVi::HiVi_stop_selected_primary_vi_chn (m_source_config.size() == %d)\n", m_source_config.size());

    for(phy_chn_num = 0; phy_chn_num < max_vi_chn_num; phy_chn_num ++)
    {
        if(0 != ((chn_mask) & (1LL<<phy_chn_num)))
        {   
            DEBUG_WARNING( "CHiVi::HiVi_stop_selected_primary_vi_chn chn %d is analoag!!! \n", phy_chn_num);

            if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
            {
                DEBUG_ERROR( "CHiVi::HiVi_stop_selected_primary_vi_chn HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
                goto _OVER_;
            }

            if(HiVi_stop_vi_chn(vi_chn) != 0)
            {
                DEBUG_ERROR( "CHiVi::HiVi_stop_selected_primary_vi_chn HiVi_stop_vi_chn FAILED(%d)(%d)\n", phy_chn_num, vi_chn);
                goto _OVER_;
            }
            std::map<int, vi_config_set_t>::iterator phy_chn_it;

            phy_chn_it = m_source_config.find(phy_chn_num);
            if (phy_chn_it != m_source_config.end())
            {
                m_source_config.erase(phy_chn_num);
            }
            
            phy_chn_it = m_primary_channel_primary_attr.find(phy_chn_num);
            if (phy_chn_it != m_primary_channel_primary_attr.end())
            {
                m_primary_channel_primary_attr.erase(phy_chn_num);
            }
            
            phy_chn_it = m_primary_channel_minor_attr.find(phy_chn_num);
            if (phy_chn_it != m_primary_channel_minor_attr.end())
            {
                m_primary_channel_minor_attr.erase(phy_chn_num);
            }
            std::map<int, int>::iterator preview_iterator;
            preview_iterator = m_preview_counter.find(phy_chn_num);
            if (preview_iterator != m_preview_counter.end())
            {
                m_preview_counter.erase(phy_chn_num);
            }
        }
    }

    ret_val = 0;

_OVER_:
    
    m_pThread_lock->unlock();
    DEBUG_MESSAGE( "CHiVi::HiVi_stop_selected_primary_vi_chn success\n");

    return ret_val;

}

int CHiVi::HiVi_vi_cover(int phy_chn_num, int x, int y, int width, int height, int cover_index)
{
    RGN_HANDLE region_handle;
    RGN_ATTR_S region_attr;
    MPP_CHN_S mpp_chn;
    RGN_CHN_ATTR_S region_chn_attr;
    HI_S32 ret_val;
    VI_CHN vi_chn;

    if((cover_index < 0) || (cover_index >= 4) || (HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0))
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_cover invalid parameter(%d)(%d)\n", phy_chn_num, cover_index);
        return 0;
    }

    Har_get_region_handle_vi(phy_chn_num, cover_index, &region_handle);

    DEBUG_MESSAGE( "CHiVi::HiVi_vi_cover(chn:%d)(x:%d, y:%d, w:%d, h:%d)(index:%d)(handle:%d)\n", phy_chn_num, x, y, width, height, cover_index, region_handle);

#if defined(_AV_PLATFORM_HI3531_) || defined(_AV_PLATFORM_HI3532_)
    const char *pMemory_name = NULL;

    pMemory_name = "ddr1";
    mpp_chn.enModId  = HI_ID_RGN;
    mpp_chn.s32DevId = 0;
    mpp_chn.s32ChnId = 0;
    if((ret_val = HI_MPI_SYS_SetMemConf(&mpp_chn, pMemory_name)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_cover HI_MPI_SYS_SetMemConf FAILED(0x%08x)\n", ret_val);
    }
#endif

#ifdef _AV_PRODUCT_CLASS_IPC_
    region_attr.enType = COVEREX_RGN;
#else
    region_attr.enType = COVER_RGN;
#endif
    if((ret_val = HI_MPI_RGN_Create(region_handle, &region_attr)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_cover HI_MPI_RGN_Create FAILED(%08x)\n", ret_val);        
        return -1;
    }
    //refreence p669
    mpp_chn.enModId = HI_ID_VIU;
    mpp_chn.s32ChnId = vi_chn;
    mpp_chn.s32DevId = 0;
    ////////////////////////////////////////////
    VI_CHN_ATTR_S stuChnAttr;
    ret_val = HI_MPI_VI_GetChnAttr( vi_chn, &stuChnAttr );
    if( ret_val == HI_SUCCESS )
    {
        //DEBUG_MESSAGE("get vi info, w=%d h=%d cor(%d %d %d %d)\n", stuChnAttr.stDestSize.u32Width, stuChnAttr.stDestSize.u32Height, x, y, width, height);
        pgAvDeviceInfo->Adi_covert_coordinateEx( stuChnAttr.stCapRect.u32Width, stuChnAttr.stCapRect.u32Height, &x, &y, 1);
        pgAvDeviceInfo->Adi_covert_coordinateEx( stuChnAttr.stCapRect.u32Width, stuChnAttr.stCapRect.u32Height, &width, &height, 1);
        //DEBUG_MESSAGE("get vi info, vi_chn = %d w=%d h=%d cor(%d %d %d %d)\n", vi_chn, stuChnAttr.stCapRect.u32Width, stuChnAttr.stCapRect.u32Height, x, y, width, height);
    }
    else
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_cover HI_MPI_VI_SetChnAttr FAILED(%08x)\n", ret_val);  
    }
    ////////////////////////////////////////////

    region_chn_attr.bShow = HI_TRUE;
#ifdef _AV_PRODUCT_CLASS_IPC_ //for LDC
    region_chn_attr.enType = COVEREX_RGN;
    region_chn_attr.unChnAttr.stCoverExChn.stRect.s32X = x / 4 * 4;//4
    region_chn_attr.unChnAttr.stCoverExChn.stRect.s32Y = y / 4 * 4;
    region_chn_attr.unChnAttr.stCoverExChn.stRect.u32Width = width/ 4 * 4;
    region_chn_attr.unChnAttr.stCoverExChn.stRect.u32Height = height / 4 * 4;

    region_chn_attr.unChnAttr.stCoverChn.u32Color = 0x0;
    region_chn_attr.unChnAttr.stCoverChn.u32Layer = cover_index;    
#else
    region_chn_attr.enType = COVER_RGN;
    //region_chn_attr.unChnAttr.stCoverChn.stRect.s32X = (x + 3) / 4 * 4;//4
    //region_chn_attr.unChnAttr.stCoverChn.stRect.s32Y = (y + 3) / 4 * 4;
    //region_chn_attr.unChnAttr.stCoverChn.stRect.u32Width = (width + 3) / 4 * 4;
    //region_chn_attr.unChnAttr.stCoverChn.stRect.u32Height = (height + 3) / 4 * 4;

    region_chn_attr.unChnAttr.stCoverChn.stRect.s32X = x / 4 * 4;
    region_chn_attr.unChnAttr.stCoverChn.stRect.s32Y = y / 4 * 4;
    region_chn_attr.unChnAttr.stCoverChn.stRect.u32Width = width/ 4 * 4;
    region_chn_attr.unChnAttr.stCoverChn.stRect.u32Height = height / 4 * 4;

    region_chn_attr.unChnAttr.stCoverChn.u32Color = 0x0;

    region_chn_attr.unChnAttr.stCoverChn.u32Layer = cover_index;
#endif
    if((ret_val = HI_MPI_RGN_AttachToChn(region_handle, &mpp_chn, &region_chn_attr)) != HI_SUCCESS)
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_cover HI_MPI_RGN_AttachToChn FAILED(0x%08x)\n", ret_val);        
        return -1;
    }

    return 0;
}


int CHiVi::HiVi_vi_uncover(int phy_chn_num, int cover_index)
{
    RGN_HANDLE region_handle;
    MPP_CHN_S mpp_chn;
    VI_CHN vi_chn;

    if((cover_index < 0) || (cover_index >= 4) || (HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0))
    {
        DEBUG_ERROR( "CHiVi::HiVi_vi_uncover invalid parameter(%d)(%d)\n", phy_chn_num, cover_index);
        return 0;
    }
    //get region handle
    Har_get_region_handle_vi(phy_chn_num, cover_index, &region_handle);

    //DEBUG_MESSAGE( "CHiVi::HiVi_vi_uncover(chn:%d)(index:%d)(handle:%d)\n", phy_chn_num, cover_index, region_handle);

    mpp_chn.enModId = HI_ID_VIU;
    mpp_chn.s32ChnId = vi_chn;
    mpp_chn.s32DevId = 0;
#ifdef _AV_PLATFORM_HI3520D_V300_
    HI_MPI_RGN_DetachFromChn(region_handle, &mpp_chn);
#else
    HI_MPI_RGN_DetachFrmChn(region_handle, &mpp_chn);
#endif
    HI_MPI_RGN_Destroy(region_handle);

    return 0;
}

int CHiVi::HiVi_set_usr_picture(int phy_chn_num, const VIDEO_FRAME_INFO_S *pVideo_frame_info)
{
    int ret_val   = -1;
    VI_CHN vi_chn = 0;
    VI_USERPIC_ATTR_S vi_userpic_attr;

    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_usr_picture chn %d is digital!!! \n", phy_chn_num);
        //return 0;
    }

    memset(&vi_userpic_attr, 0, sizeof(VI_USERPIC_ATTR_S));
    vi_userpic_attr.bPub                 = HI_TRUE;
    //if(OSD_TEST_FLAG == pgAvDeviceInfo->Adi_product_type())//6AII_AV12机型，视频丢失时，背景显示黑色//
    //{
	//	vi_userpic_attr.enUsrPicMode		 = VI_USERPIC_MODE_BGC;
	//	vi_userpic_attr.unUsrPic.stUsrPicBg.u32BgColor	= 0x0;
    //}
    //else
    {
		vi_userpic_attr.enUsrPicMode		 = VI_USERPIC_MODE_PIC;
		memcpy(&vi_userpic_attr.unUsrPic.stUsrPicFrm, pVideo_frame_info, sizeof(VIDEO_FRAME_INFO_S));
    }

    HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL);
#if defined(_AV_PLATFORM_HI3520D_V300_)
    //vi_userpic_attr.unUsrPic.stUsrPicFrm.stVFrame.enCompressMode=COMPRESS_MODE_NONE;
   // vi_userpic_attr.unUsrPic.stUsrPicFrm.stVFrame.enVideoFormat=VIDEO_FORMAT_LINEAR;
    DEBUG_CRITICAL("\ncxliu,enCompressMode=%x,enVideoFormat=%x\n",vi_userpic_attr.unUsrPic.stUsrPicFrm.stVFrame.enCompressMode,
	vi_userpic_attr.unUsrPic.stUsrPicFrm.stVFrame.enVideoFormat);
#endif
    if ((ret_val = HI_MPI_VI_SetUserPic(vi_chn, &vi_userpic_attr)) != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_usr_picture HI_MPI_VI_SetUserPic FAILED! (vi_chn:%d) (0x%08x)\n", vi_chn, ret_val);
        return -1;
    }
    
    return 0;
}

int CHiVi::HiVi_insert_picture(int phy_chn_num, bool show_or_hide)
{
    int ret_val         = -1;
    VI_CHN vi_chn       = 0;

    HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL);

    if (show_or_hide)
    {
        if((ret_val = HI_MPI_VI_DisableChnInterrupt(vi_chn)) != HI_SUCCESS)
        {
            DEBUG_ERROR( "CHiVi::HiVi_insert_picture HI_MPI_VI_DisableChnInterrupt FAILED! (vi_chn:%d) (0x%08x)\n", vi_chn, ret_val);
        }
        
        if ((ret_val = HI_MPI_VI_EnableUserPic(vi_chn)) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_insert_picture HI_MPI_VI_EnableUserPic FAILED! (vi_chn:%d) (0x%08x)\n", vi_chn, ret_val);
            return -1; 
        }
    }
    else
    {
        if((ret_val = HI_MPI_VI_EnableChnInterrupt(vi_chn)) != HI_SUCCESS)
        {
            DEBUG_ERROR( "CHiVi::HiVi_insert_picture HI_MPI_VI_EnableChnInterrupt FAILED! (vi_chn:%d) (0x%08x)\n", vi_chn, ret_val);
        }
        if ((ret_val = HI_MPI_VI_DisableUserPic(vi_chn)) != 0)
        {
            DEBUG_ERROR( "CHiVi::HiVi_insert_picture HI_MPI_VI_DisableUserPic FAILED! (vi_chn:%d) (0x%08x)\n", vi_chn, ret_val);
            return -1; 
        }
    }
    return 0;
}

int CHiVi::HiVi_set_cscattr( int phy_chn_num, unsigned char u8Luma, unsigned char u8Hue, unsigned char u8Contr, unsigned char u8Satu )
{
    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_WARNING( "CHiVi::HiVi_set_cscattr chn %d is digital!!! \n", phy_chn_num);
        return 0;
    }
    
 #if defined(_AV_PLATFORM_HI3518C_)
    if( phy_chn_num >= pgAvDeviceInfo->Adi_max_channel_number() )
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_cscattr channel error, channel is:%d\n", phy_chn_num );
        return -1;
    }

    u8Luma = (u8Luma>64?64:u8Luma);
    u8Hue = (u8Hue>64?64:u8Hue);
    u8Contr = (u8Contr>64?64:u8Contr);
    u8Satu = (u8Satu>64?64:u8Satu);


#if 0
    //analog gain value:-16.5+1.5*new_gain_micl
    HI_U32 old_value;
    unsigned char new_gain_micl = (unsigned char)(u8Satu/2);
    HI_U32 new_value = 0;

    HI_MPI_SYS_GetReg(0x20050068, &old_value);
    printf("[dhdong]the old value is:0x%x, old gain_micl is:%d \n", old_value, new_gain_micl);

    old_value = old_value&(0xFFFF8383);
    new_gain_micl = (new_gain_micl<<2)&(0x7C);
    new_value = old_value | (new_gain_micl << 8) | new_gain_micl;
    printf("[dhdong]the new value is:0x%x, new gain_micl is:%d \n", new_value, new_gain_micl);

    HI_MPI_SYS_SetReg(0x20050068, new_value);

    //digital gain value(db):30 - new_gain_vol
    HI_U32 old_vol;
    unsigned char new_gain_vol = u8Contr;
    HI_U32 new_vol = 0;

    HI_MPI_SYS_GetReg(0x20050078, &old_vol);
    printf("[dhdong]the old digital gain value is:0x%x, new gain is:%d \n", old_vol, new_gain_vol);

    old_vol = old_vol&(0x0000FFFF);
    new_gain_vol = (new_gain_vol)&(0x7F);
    new_vol = old_vol | (new_gain_vol << 16) | (new_gain_vol << 24);
    printf("[dhdong]the digital vol new value is:0x%x, new gain_micl is:%d \n", new_vol, new_gain_vol);

    HI_MPI_SYS_SetReg(0x20050078, new_vol);

#endif

    VI_CSC_TYPE_E eCscType = VI_CSC_TYPE_709;

    VI_CSC_ATTR_S stuParam;
    memset( &stuParam, 0, sizeof(stuParam) );

    m_u8LastSatu = u8Satu;
    
    stuParam.enViCscType = eCscType;
    stuParam.u32LumaVal = u8Luma*100/64;
    stuParam.u32HueVal = u8Hue*100/64;
    stuParam.u32ContrVal = u8Contr*100/64;
    stuParam.u32SatuVal = u8Satu*100/64;
    if( m_u8LastColorGrayMode != 0 )
    {
        stuParam.u32SatuVal = 0;
    }
    
    HI_S32 sRet = HI_MPI_VI_SetCSCAttr( phy_chn_num, &stuParam );
    if( sRet != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_cscattr HI_MPI_VI_SetCSCAttr error with 0x%08x\n", sRet );
        return -1;
    }
 #endif
    return 0;
}

int CHiVi::Hivi_set_color_gray_mode( int phy_chn_num, unsigned char u8Mode )
{
    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_WARNING( "CHiVi::Hivi_set_color_gray_mode chn %d is digital!!! \n", phy_chn_num);
        return 0;
    }
#ifdef _AV_PLATFORM_HI3518C_
    VI_CSC_ATTR_S stuParam;
    memset( &stuParam, 0, sizeof(stuParam) );

    HI_S32 sRet = HI_MPI_VI_GetCSCAttr( phy_chn_num, &stuParam );
    if( sRet != HI_SUCCESS )
    {
       DEBUG_ERROR( "CHiVi::Hivi_set_color_gray_mode HI_MPI_VI_GetCSCAttr error with 0x%08x\n", sRet );
       return -1;
    }

    stuParam.u32SatuVal = 0;
    if( u8Mode == 0 )
    {
        stuParam.u32SatuVal = m_u8LastSatu*100/64;
    }
    m_u8LastColorGrayMode = u8Mode;

    sRet = HI_MPI_VI_SetCSCAttr( phy_chn_num, &stuParam );
    if( sRet != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::Hivi_set_color_gray_mode HI_MPI_VI_SetCSCAttr error with 0x%08x\n", sRet );
        return -1;
    }
#endif
    return 0;
}

int CHiVi::HiVi_set_mirror_flip( int phy_chn_num, bool bIsMirror, bool bIsFlip )
{
    VI_CHN_ATTR_S stChnAttr;
    HI_S32 sRet = HI_SUCCESS;

    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_CRITICAL( "CHiVi::HiVi_set_mirror_flip chn %d is digital!!! \n", phy_chn_num);
        return 0;
    }

    VI_CHN vi_chn;
    HiVi_get_primary_vi_info(phy_chn_num,NULL,&vi_chn,NULL);
    
    sRet = HI_MPI_VI_GetChnAttr( vi_chn, &stChnAttr );
    if( sRet != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_mirror_flip HI_MPI_VI_GetChnAttr failed with 0x%08x\n", sRet );
        return -1;
    }

    stChnAttr.bMirror = (bIsMirror ? HI_TRUE : HI_FALSE);
    stChnAttr.bFlip = (bIsFlip ? HI_TRUE : HI_FALSE);

    sRet = HI_MPI_VI_SetChnAttr( vi_chn, &stChnAttr );
    if( sRet != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::HiVi_set_mirror_flip HI_MPI_VI_SetChnAttr failed with 0x%08x\n", sRet );
        return -1;
    }

    return 0;
}


int CHiVi::Hivi_get_vi_frame( int phy_chn_num, VIDEO_FRAME_INFO_S* pstuFrameInfo )
{
    int vi_chn = 0;

    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_ERROR( "CHiVi::Hivi_get_vi_frame chn %d is digital!!! \n", phy_chn_num);
        return 0;
    }
    
    if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::Hivi_get_vi_frame HiVi_get_primary_vi_info failed\n" );
        return -1;
    }
#ifdef _AV_PLATFORM_HI3520D_V300_
    HI_S32 s32Ret = HI_MPI_VI_GetFrame( vi_chn, pstuFrameInfo,200);
#else
    HI_S32 s32Ret = HI_MPI_VI_GetFrameTimeOut( vi_chn, pstuFrameInfo, 200 );
#endif
    if( HI_SUCCESS != s32Ret )
    {
        DEBUG_ERROR( "CHiVi::Hivi_get_vi_frame HI_MPI_VI_GetFrameTimeOut failed witch 0x%08x\n", s32Ret );
        return -1;
    }

    return 0; 
}

int CHiVi::Hivi_release_vi_frame( int phy_chn_num, VIDEO_FRAME_INFO_S* pstuFrameInfo )
{
    int vi_chn = 0;
    if (pgAvDeviceInfo->Adi_get_video_source(phy_chn_num) == _VS_DIGITAL_)
    {
        DEBUG_WARNING( "CHiVi::Hivi_release_vi_frame chn %d is digital!!! \n", phy_chn_num);
        return 0;
    }
    
    if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::Hivi_get_vi_frame HiVi_get_primary_vi_info failed\n" );
        return -1;
    }

    if( HI_MPI_VI_ReleaseFrame(vi_chn, pstuFrameInfo) != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::Hivi_get_vi_frame HI_MPI_VI_ReleaseFrame failed\n" );
        return -1;
    }

    return 0;
}

#if defined(_AV_PRODUCT_CLASS_IPC_)
int CHiVi::Hivi_set_ldc_attr( int phy_chn_num, const VI_LDC_ATTR_S* pstLDCAttr )
{
    DEBUG_MESSAGE( "CHiVi::Hivi_set_ldc_attr channel=%d enable=%d type=%d Xoff=%d Yoff=%d ratio=%d\n", 
        phy_chn_num, pstLDCAttr->bEnable, pstLDCAttr->stAttr.enViewType, pstLDCAttr->stAttr.s32CenterXOffset
        , pstLDCAttr->stAttr.s32CenterYOffset, pstLDCAttr->stAttr.s32Ratio);
    
    HI_S32 sRet = HI_MPI_VI_SetLDCAttr( phy_chn_num, pstLDCAttr );
    if( sRet != HI_SUCCESS )
    {
        DEBUG_ERROR( "CHiVi::Hivi_set_ldc_attr HI_MPI_VI_SetLDCAttr failed with 0x%x\n", sRet );
        return -1;
    }

    return 0;
}
#endif

int CHiVi::HiVi_one_vi_dev_control_by_chn(bool start_flag, int phy_chn_num, vi_config_set_t *pSource/* = NULL*/)
{
    int retval = -1;
    VI_DEV videv;
    VI_INTF_MODE_E vi_intf_mode = VI_MODE_BUTT;
    VI_WORK_MODE_E vi_work_mode = VI_WORK_MODE_BUTT;
    VI_SCAN_MODE_E vi_scan_mode = VI_SCAN_BUTT;
    std::bitset<_MAX_VI_DEV_NUM_> vi_dev_state;
    HiVi_get_vi_dev_info(vi_dev_state);
    HI_U32 mask0 = 0, mask1 = 0;

    if(pgAvDeviceInfo->Adi_weather_phychn_sdi(phy_chn_num) == false)
    {
        DEBUG_ERROR( "CHiVi::HiVi_one_vi_dev_control_by_chn chn %d is not sdi\n", phy_chn_num);
        goto _OVER_;
    }
    if(HiVi_get_primary_vi_info(phy_chn_num, &videv, NULL, NULL) != 0)
    {
        DEBUG_ERROR( "CHiVi::HiVi_stop_all_primary_vi_chn HiVi_get_primary_vi_info FAILED(%d)\n", phy_chn_num);
        goto _OVER_;
    }

    if(!vi_dev_state.test(videv))
    {
        goto _OVER_;
    }
    if(pSource != NULL)
    {
        if(pSource->progress == true)
        {
            vi_scan_mode = VI_SCAN_PROGRESSIVE;
        }
        else
        {
            vi_scan_mode = VI_SCAN_INTERLACED;
        }
    }
    
    retval = HiVi_vi_dev_control_V1(videv, start_flag, vi_intf_mode, vi_work_mode, vi_scan_mode, mask0, mask1);

_OVER_:

    return retval;
}

#if defined(_AV_PRODUCT_CLASS_IPC_)
int CHiVi::HiVi_get_video_size(IspOutputMode_e iom, int *pWidth, int *pHeight)
{
    return pgAvDeviceInfo->Adi_get_video_size(iom,pWidth,pHeight);
}

#if defined(_AV_PLATFORM_HI3518C_)
 int CHiVi::HiVi_set_chn_capture_coordinate_offset(int phy_chn_num, int x_offset, int y_offset)
 {
    VI_CHN vi_chn;
    if(HiVi_get_primary_vi_info(phy_chn_num, NULL, &vi_chn) != 0)
    {
        return -1;
    }
    VI_CHN_ATTR_S chn_attr;
    HI_S32 nRet = HI_FAILURE;

    memset(&chn_attr, 0, sizeof(VI_CHN_ATTR_S));
    nRet = HI_MPI_VI_GetChnAttr(vi_chn, &chn_attr);
    if(nRet != HI_SUCCESS)
    {
        DEBUG_ERROR("HI_MPI_VI_GetChnAttr failed! errorcode:0x#x \n", nRet);
        return -1;
    }

    sensor_e st  = ST_SENSOR_DEFAULT;
    unsigned char chn_id = 0;
    vi_capture_rect_s rect;
    int width = 0;
    int height = 0;

    memset(&rect, 0, sizeof(vi_capture_rect_s));
    if(PTD_712_VD == pgAvDeviceInfo->Adi_product_type())
    {
        if(0 != pgAvDeviceInfo->Adi_get_ipc_work_mode())
        {
            chn_id = 1;
        }
    }
    st = pgAvDeviceInfo->Adi_get_sensor_type();
    pgAvDeviceInfo->Adi_get_vi_config(st, chn_id, pgAvDeviceInfo->Adi_get_vi_max_resolution(), rect);
    HiVi_get_video_size(pgAvDeviceInfo->Adi_get_isp_output_mode(), &width, &height);

    int new_x = rect.m_vi_capture_x + x_offset;
    int new_y = rect.m_vi_capture_y + y_offset;

    if(new_x<0)
    {
        new_x = 0;
    }

    if(new_x > rect.m_vi_capture_width - width)
    {
        new_x = rect.m_vi_capture_width - width;
    }
    
    if(new_y < 0)
    {
        new_y = 0;
    } 
    if(new_y > rect.m_vi_capture_height - height)
    {
        new_y = rect.m_vi_capture_height - height;
    }

    chn_attr.stCapRect.s32X = new_x;
    chn_attr.stCapRect.s32Y = new_y;

    chn_attr.stCapRect.s32X =  (chn_attr.stCapRect.s32X + 4 -1)/4*4;
    chn_attr.stCapRect.s32Y = (chn_attr.stCapRect.s32Y + 4 -1)/4*4;

    DEBUG_MESSAGE("HI_MPI_VI_SetChnAttr new x:%d y:%d \n", chn_attr.stCapRect.s32X, chn_attr.stCapRect.s32Y);
    nRet = HI_MPI_VI_SetChnAttr(vi_chn, &chn_attr);
    if(nRet != HI_SUCCESS)
    {
        DEBUG_ERROR("HI_MPI_VI_SetChnAttr failed! errorcode:0x#x \n", nRet);
        return -1;        
    }
    
    return 0;
 }
#endif

int CHiVi::HiVi_vi_query_chn_state(int phy_chn_num, VI_CHN_STAT_S& chn_state)
{
    VI_CHN chn;
    VI_DEV dev;
    
    if(-1 != HiVi_get_primary_vi_info(phy_chn_num, &dev, &chn))
    {
        if(HI_SUCCESS != HI_MPI_VI_Query(chn, &chn_state))
        {
            DEBUG_ERROR("HI_MPI_VI_Query failed! \n");
            return -1;
        }

        return 0;
    }

    return -1;
}


	
#endif

