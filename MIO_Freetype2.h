/**
  ****************** (C) COPYRIGHT 2018 Nova Co.,Ltd  *******************
  * @File            : MIO_Freetype2.h.h
  * @Author          : zhangjianan
  * @Version         : V1.0
  * @Date            : 06/21/2018
  * @Description     : This file provides a .....
  ***************************************************************
  * @attention
  *
  * History<br>
  * version     date          auther           description<br>
  * V1.0.0     2020.03.16     zhangjianan     Initial Version<br>
  *
  ***************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __MIO_FREETYPE2_H__
#define __MIO_FREETYPE2_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum 
{
    En_SourceHanSans_CH,       //˼Դ����
    En_SimSun,                 //����
    En_MicrosoftYahei,         //΢���ź�
    En_KaiTi,                  //����
    En_Helvetica,
    En_Didot,
    En_Arial,
    En_calibri,
    En_Frutura,
    En_Times,
    En_Custom_1,
    En_Custom_2,
    En_Custom_3,
    En_Custom_4,
    En_Custom_5,
}En_Font_t;

typedef enum
{
    En_LeftAlignment,
    En_CenterAlignment,
    En_RightAlignment,
}En_Alignment;

int DrawFont(int32_t width,int32_t height,int32_t font_height,wchar_t  *word,
    unsigned char *image,uint32_t image_len,En_Font_t font,char *font_name,int space,En_Alignment alig,int32_t invalid_width);
int JudgeStr(int32_t width,int32_t height,int32_t font_height,wchar_t  *word,
    En_Font_t font,char *font_name,int space,int32_t invalid_width);

#ifdef __cplusplus
}
#endif

#endif /* __MIO_FREETYPE2_H__ */
/************************ (C) COPYRIGHT END OF FILE****/