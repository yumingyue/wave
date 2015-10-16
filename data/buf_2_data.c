/*=============================================================================
#
# Author: 杨广华 - edesale@qq.com
#
# QQ : 374970456
#
# Last modified: 2015-10-12 21:55
#
# Filename: buf_2_data.c
#
# Description:将buf中的信息填充到结构体中
# 
#	命名规则为:buf_2_sturct(const u8* buf, const u32 len, struct*)
#   其中buf为将要被填充的字节流，len为字节流buf大小，struct为将要填充的结构体
#	函数返回值 0， 填充失败； 大于0， 填充了多少字节
=============================================================================*/
#include"data.h"
#include<stdlib.h>
#include<stddef.h>

#define get8(addr)  *( (u8*)addr  )
#define get16(addr) *( (u16*)addr )
#define get32(addr) *( (u32*)addr )
#define get64(addr) *( (u64*)addr )

#define CURRETN_VERSION 2

/**	向array 中 buf所指向的内存填充
 *	@array_buf  array_buf指向需要被填充的内存的首地址，
 *	@mbuf mbuf指向将要被填充进buf所指向内存的网络流的首地址
 *	@len 所需要填充的个数
 *	YGH
 */

static void fill_buf8(u8* array_buf, u8* mbuf, u16 len){
	u16 i;
	for(i = 0; i< len;i++){
		*array_buf = *mbuf;
		array_buf++;
		mbuf++;
	}
}
/**获取变长编码variable-length vectors with variable-length length enconding （协议6.1.6.2节）的头部ll段的编码bit位数 
 *	@mbuf 表示长度的网络流的首地址
 *  返回值：数据的长度，类型为U16，data.h 中定义len类型为u16
 *  YGH
 */
static u16 head_bit_num(u8* mbuf){
	u8 ll;
	ll = get8(mbuf);
	if(ll <= 0x7f)
		return 1;
	else if(ll <= 0xbf)
		return 2;
	else if(ll <= 0xdf)
		return 3
	else if(ll <= 0xef)
		return 4;
	else if(ll <= 0xf7)
		return 5; 
	else if(ll <= 0xfb)
		return 6;
	else if(ll <= 0xfd)
		return 7;
	else(ll <= 0xfe)
		return 8;
}
/** 计算variable-length vectors with variable-length length enconding中 数据的个数
 *	@full_encoding_length数据个数
 *	@mbuf 头部的起始地址
 *	返回值： 头部长度
 *	YGH
 */
static u32 variablelength_data_num(u8* mbuf,u16 full_encoding_length){
	u16 full_encoding_length16;
	u32 full_encoding_length32;
	u64 full_encoding_length64;
	switch(full_encoding_length){
		case 1:
			full_encoding_length16 = get16(mbuf);
			full_encoding_length16 = be_to_host16(full_encoding_length16);
			full_encoding_length16 = full_encoding_length16>>8;
			return full_encoding_length16;
		case 2;
			full_encoding_length16 = get16(mbuf);
			full_encoding_length16 = be_to_host16(full_encoding_length16);
			return full_encoding_length16 & 0x3fff;
		case 3;
			full_encoding_length32 = get32(mbuf);
			full_encoding_length32 = be_to_host32(full_encoding_length32;)
			full_encoding_length32 = full_encoding_length32>>8;
			full_encoding_length32 = full_encoding_length32 & 0x003fffff;
			return (u16)full_encoding_lenth32;
		case 4;
			full_encoding_length32 = get32(mbuf);
			full_encoding_length32 = be_to_host32(full_encoding_length32);
			full_encoding_length32 = full_encoding_length32;
			full_encoding_length32 = full_encoding_length32 & 0x0fffffff;
			return (u16)full_encoding_length32;
		case 5;
			full_encoding_length64 = get64(mbuf);
			full_encoding_length64 = be_to_host64(full_encoding_length64);
			full_encoding_length64 = full_encoding_length64 >> 24;
			full_encoding_length64 = full_encoding_length64 & 0x00000007ffffffff;
			return (u16)full_encoding_length64
		case 6:
			full_encoding_length64 = get64(mbuf);
			full_encoding_length64 = be_to_host64(full_encoding_length64);
			full_encoding_length64 = full_encoding_length64 >> 16;
			full_encoding_length64 = full_encoding_length64 & 0x000003ffffffffff;
			return (u16)full_encoding_length64;
		case 7:
			full_encoding_length64 = get64(mbuf);
			full_encoding_length64 = be_to_host64(full_encoding_length64);
			full_encoding_length64 = full_encoding_length64 >> 8;
			full_encoding_length64 = full_encoding_length64 & 0x0001ffffffffffff;
			return (u16)full_encoding_length64;
		case 8:
			full_encoding_length64 = get64(mbuf);
			full_encoding_length64 = be_to_host64(full_encoding_length64);
			full_encoding_length64 = full_encoding_length64 & 0x00ffffffffffffff;
			return (u16)full_encoding_length64;
}
/**
 * buf_to 1
 */
static u32 buf_2_time64_with_standard_deviation(const u8* buf, const u32 len, 
				time64_with_standard_deviation* time64_with_standard_deviation){
	u8* mbuf = buf;
	u32 size = len;
	//u32 used_length = 0;
	
	if(size < 9)
		return 0;
	
	time64_with_standard_deviation->time = get64(mbuf);
	time64_with_standard_deviation->time = be_to_host64(time64_with_standard_deviation->time);  //定义在wave/utils/common.h中
	mbuf += 8;
	size -= 8;
	//used_length += 4;

	time64_with_standard_deviation->long_std_dev = get8(mbuf);
	mbuf += 1;
	size -= 1;
	//used_length += 1;
	
	return len - size;
}
/**
 *	buf_to 2
 */

static u32 buf_2_tbsdata_extension(const u8* buf, const u32 len, tbsdata_extion* tbsdata_extsion){
	u8* mbuf = buf;
	u32 size = len;
	u16 bitnum;//代表头部编码长度
	
	if(size < 2)
		return 0;
	tbsdata_extension->type = get8(mbuf);
	mbuf += 1;
	size -= 1;
			

	bitnum = head_bit_num(mbuf);
	tbsdata_extension->value.len = variablelength_data_num(mbuf, bitnum);
	if(size < tbsdata_extension->value.len*sizeof(u8)+bitnum)
		return 0;
		
	mbuf += bitnum;
	size -= bitnum;
	tbsdata_extension->value.buf = (u8*)malloc(sizeof(u8)*tbsdata_extension->value.len);
	if(NULL == tbsdata_extension->value.buf )
		return 0;
	fill_buf8(buf, mbuf, tbsdata_extension->value.len);
	mbuf+= tbsdata_extension->value.len *sizeof(u8);
	size -= tbsdata_extension->value.len * sizeof(u8);
	return len - size;
}
/**
 *	buf_to 3
 */
static u32 buf_2_three_d_location(const u8* buf, const u32 len, three_d_location* three_d_location){
	u8* mbuf = buf;
	u32 size = len;
	//u32 used_length = 0;

	if(size >= 4 ){
		three_d_location->latitude	= get32(mbuf);
		be_to_host32(three_d_location->latitude)
		mbuf += 4;
		size -= 4;
	//	used_lenth += 4;
	}
	if(size >= 4){
		three_d_location->longitude = get32(mbuf);
		be_to_host32(three_d_location->longitude);
		mbuf += 4;
		size -= 4;
	//	used_lenth += 4;
	}
	
	if(size >= 2){
		three_d_location->elevation[1] = get8(mbuf);
		mbuf++;
		three_d_location->elevation[2] = get8(mbuf);
		mbuf++;
		size -= 2;
	//	used_lenth += 2;
		return len - size;
	}
	return 0;
}

/**
 *	buf_to 4
 */
static u32 buf_2_hashedid8(const u8* buf, const u32 len, hashedid8* hashedid8){
	u8* mbuf = buf; 
	u32 size = len;
	int i;
	if (size < 8)
		return 0;
	for(i = 0; i < 8; i++ ){
		hashedid8->hashedid[i] = get8(mbuf);
		mbuf++;
	}
	return 8;
}

/**
 *	buf_to 5
 *	@pk_algorithm 外部传入参数
 *	@field_size 外部传入参数，注意在填充结构体时，外部传入参数是不需要真正填充进结构体的
 *	因为结构体定义中，并没有extern数据，而且编码中，extern也没有编在buf所指像的网络流中
 */
static u32 buf_2_elliptic_curve_point(const u8* buf, const u32 len, 
				elliptic_curve_point* elliptic_curve_point, pk_algorithm pk_algorithm){
	u8 mbuf = buf;
	u32 size = len;
	u16 bitnum;

	if(size < 2)
		return 0;
	elliptic_curve_point->type = get8(mbuf);
	mbuf++;
	size--;

	bitnum = head_bit_num(mbuf);
	elliptic_curve_point->x.len = variablelength_data_num(mbuf, bitnum);
	mbuf += bitnum;
	//size-=bitnum;
	if(size < bitnum + elliptic_curve_point->x.len*sizeof(u8))
		return 0;
	elliptic_curve_point->x.buf = (u8*)malloc(sizeof(u8)* elliptic_curve_point->x.len);
	fill_buf8(elliptic_curve_point->x.buf, mbuf, elliptic_curve_point->x.len);
	mbuff += elliptic_curve_point->x.len * sizeof(u8);
	size -= elliptic_curve_point->x.len * sizeof(u8);

	if(elliptic_curve_point->type == UNCOMPRESSED){
		bitnum = head_bit_num(mbuf);
		elliptic_curve_point->u.y.len = variablelength_data_num(mbuf, bitnum);
		mbuf += bitnum;
		//size-=bitnum
		if(size < bitnum + elliptic_curve_point->u.y.len*sizeof(u8)
			return 0;
		elliptic_curve_point->u.y.buf = (u8*)malloc(sizeof(u8)* elliptic_curve_point->u.y.len);
		fill_buf8(elliptic_curve_point->u.y.buf, mbuf, elliptic_curve_point->u.y.len);
		mbuff += elliptic_curve_point->u.y.len * sizeof(u8);
		size -= elliptic_curve_point->u.y.len * sizeof(u8);
	}
	return len - size;
}
/**
 *	buf_to 6
 *
 */
static u32 buf_2_ecdsa_signature(const u8* buf,const u32 len,ecdsa_signature* ecdsa_signature, 
		pk_algorithm pk_algorithm){
	u8* mbuf = buf;
	u16 bitnum ;

	u32 size = len;
	u32 elliptic_length;
	elliptic_length = buf_2_elliptic_curve_point(mbuf,len, &ecdsa_signature->elliptic_curve_point, pk_algorithm);
	if(0 == elliptic_length)
		return 0;
	mbuf += elliptic_length;
	size -= elliptic_length;
	
	if(size < 2)
		return 0;
	bitnum = head_bit_num(mbuf);
	mbuf += bitnum;
	size -= bitnum;
	ecdsa_signature->s.len = variablelength_data_num(mbuf, bitnum);

	if(size < ecdsa_signature->s.len*sizeof(u8))
		return 0;
	ecdsa_signature->s.buf = (u8*)malloc(sizeof(u8)*ecdsa_signature->s.len);
	fill_buf8(ecdsa_signature->s.buf, mbuf, ecdsa_signature->s.len);//填充buff
	mbuf += ecdsa_signature->s.len;
	size -= ecdsa_signature->s.len;
	
	return len - size;
}

 
//buf_to 7
static u32 buf_2_signature(const u8* buf,const u32 len,signature*,signature,pk_algorithm pk_algorithm){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
	u32 ecdsa_signature_length;

	if(signature->pk_algorithm!=ecdsa_signature&&signature->pk_algorithm!=ecdsa_signature){
        ecdsa_signature_length=buf_2_ecdsa_signature(mbuf,len,&signature->u.ecdsa_signature,pk_algorithm);
		if(0==ecdsa_signature_length)
		 return -1;
		mbuf+=ecdsa_signature_length;
		size-=ecdsa_signature_length;
   
	  if(size<2)
	   return -1;
	bitnum=head_bit_num(mbuf);
	mbuf+=bitnum;
	size-=bitnum;
	signature->u.signature.len=variablelength_data_num(mbuf,bitnum);
	if(size<signature->u.signature.len*sizeof(u8))
		return -1;
	signature->u.signature.buf=(u8*)malloc(sizeof(u8)*signature->u.signature.len);
	fill_buf8(signature->u.signature.buf,mbuf,signature->u.signature.len);
	mbuf+=signature->u.signature.len;
	size-=signature->u.signature.len;

	return len-size;
	}
}

//buf_to 8
static u32 buf_2_public_key(conse u8* buf,const u32 len,public_key* public_key,pk_algorithm pk_algorithm){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
	
//	if(size< )
		return -1;
	public_key->algorithm=get8(mbuf); 
	mbuf++;
	size--;
 
	if(public_key->pk_algorithm=ECDSA_NISTP224_WITH_SHA224||public_key->pk_algorithm=ECDSA_NISTP256_WITH_SHA256){
	u32 elliptic_length;
    elliptic_length=buf_2_elliptic_curve_point(mbuf,len,&public_key->u.elliptic_curve_point,pk_algorithm);
	if(0==elliptic_length)
		return -1;
	bitnum=head_bit_num(mbuf);
	mbuf+=bitnum;
	size-=bitnum;
	}

	if(public_key->pk_algorithm==ECIES_NISTP256){
		if(size< )
			return -1;
		public_key->supported_symm_alg=get8(mbuf);
		mbuf++;
		size--;
    
	u32 elliptic_length2;
	  elliptic_length2=buf_2_elliptic_curve_point(mbuf,len,&public_key->u.ecies_nistp256.elliptic_curve_point,pk_algorithm);
	  if(0==elliptic_length2)
		  return -1;
	  bitnum=head_bit_num(mbuf);
	  mbuf+=bitnum;
	  size-=bitnum;
     }

	if(public_key->pk_algorithm!=ECDSA_NISTP224_WITH_SHA224&&public_key->pk_algorithm!=ECDSA_NISTP256_WITH_SHA256&&public_key->ECIES_NISTP256){
		if(size<1)
			return -1;
		bitnum=head_bit_num(mbuf);
		mbuf+=bitnum;
		size-=bitnum;
        public_key->u.other_key.len=variablelength_data_num(mbuf,bitnum);
		if(size<public_key->u.other_key.len*sizeof(u8))
			return -1;
		public_key->u.other_key.buf=(u8*)malloc(sizeof(u8)*public_key->u.other_key.len);
		mbuf+=public_key->u.other_key.len;
		size-=public_key->u.other_key.len;
	}
		return len-size;
}
    

//buf_2 9
static u32 buf_2_two_d_location(const u8* buf,const u32 len,two_d_location* two_d_location){
	u8 mbuf=buf;
	u32 size=len;
	
	if(size<8)
		return -1;
     
	two_d_location->latitude=get32(mbuf);
	two_d_location->latitude=be_to_host32(two_d_location->time);

	mbuf+=4;
	size-=4;

	two_d_location->longitude=get32(mbuf);
	two_d_location->longitude=be_to_host32(two_d_location->longitude);

	mbuf+=4;
	size-=4;

	return len-size;

}


//buf_to 10
static u32 buf_2_rectangular_region(const u8* buf,const u32 len,rectangular_region* rectangular_region){
  u8* mbuf=buf;
  u32 size=len;
  u32 rectangular_length1;
  u32 rectangular_length2;

  rectangular_length1=buf_2_two_d_location(mbuf,len,&rectangular_region->north_west);
  if(0==rectangular_length1)
	  return -1;
  mbuf+=rectangular_length1;
  size-=rectangular_length1;

  rectangular_length2=buf_2_two_d_location(mbuf,len,&rectangular_region->south_east);
  if(0==rectangular_length2)
	  return -1;
  mbuf+=rectangular_length2;
  size-=rectangular_length2;

  return len-size;
}
  

//buf_to 11
static u32 buf_2_circular_region(const u8* buf,const u32 len,circular_region* circular_region){
  u8* mbuf=buf;
  u32 size=len;
  u32 circular_length;

  circular_length=buf_2_circular_region(mbuf,len,&circular_region->center);
	  if(0==circular_length)
		  return -1;
  mbuf+=circular_length;
  size-=circular_length;

  if(size<2)
	  return -1;
  circular_region->radius=get16(mbuf);
  circular_region ->radius=be_to_host16(circular_region->radius);
  mbuf+=2;
  size-=2;

  return len-size;
}

//buf_to 12
static u32 buf_2_geographic_region(const u8* buf,const u32 len,geographic_region*,geographic_region){

	u8* mbuf=buf;
	u32 size=len;
	u32 geographic_length;
	u32 polygonal_length;
	u16 bitnum;

//	if(size< )
		return -1;
	geographic_region->region_type=get8(mbuf);
	mbuf++;
	size--;
  
	switch(geographic_region->region_type){
	case FROM_ISSUER:
	case CIRCLE:
	geographic_length=buf_2_circular_region(mbuf,len,&geographic_region->circular_region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
	  break;
	case RECTANGLE:
  /*    bitnum=head_bit_num(mbuf);
	  geographic_region->u.rectangular_region.len=variablelength_data_num(mbuf,bitnum);
	  if(size<geographic_region->u.rectangular_region.len*sizeof(u8)+bitnum)
		  return -1;
	  mbuf+=bitnum;
	  size-=bitnum;
	  geographic_region->u.rectangular_region.buf=(u8*)malloc(sizeof(u8)*geographic_region->u.rectangular_region.len);
	  if(NULL==geographic_region->u.rectangular_region.buf)
		  return -1;
      fill_buf8(buf,mbuf,geographic_region->u.rectangular_region.len);
	  mbuf+=geographic_region->u.rectangular_region.len*sizeof(u8);
	  size-=geographic_region->u.rectangular_region.len*sizeof(u8);
	  return len-size;
      break;  */
      case POLYGON:
   // polygonal_length=bu   two_d_location* polygonal_length 
      break;
      default:
      bitnum=head_bit_num(mbuf);
	  geographic_region->u.other_region.len=variablelength_data_num(mbuf,bitnum);
	  if(size<geographic_region->u.other_region.len*sizeof(u8)+bitnum)
		  return -1;
	  mbuf+=bitnum;
	  size-=bitnum;
	  geographic_region->u.other_region.buf=(u8*)malloc(sizeof(u8)*geographic_region->u.other_region.len);
	  if(NULL==geographic_region->u.other_region.buf)
		  return -1;
	  fill_buf8(buf,mbuf,geographic_region->u.other_region.len);
	  mbuf+=geographic_region->u.other_region.len*sizeof(u8);
	  size-=geographic_region->u.other_region.len*sizeof(u8);
	  return len-size;
	  break;
	}
}

//buf_2  13

static u32 buf_2_psid_priority(const u8* buf,const u32 len,psid_priority* psid_priority){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
   
//	if(size< )
		return -1;
  //psid  notunderstand
    psid_priority->max_priority=get8(mbuf);
	mbuf++
	size--;

	return len-size;
}


//buf_2 14
static u32 buf_2_psid_priority_array(const u8* buf,const u32 len,psid_priority_array* psid_priority_array){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

//	if(size<  )
		return -1;

	psid_priority_array->type=get8(mbuf);
	mbuf++;
	size--;
  
   switch(psid_priority_array->type){
	case ARRAY_TYPE_SPECIFIED:
/*	   bitnum=head_bit_num(mbuf);
	   psid_priority->u.permissions_list.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_priority->u.permissions_list.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_priority_array->u.permissions_list.buf=(u8*)malloc(sizeof(u8)*psid_priority->u.permissions_list.len);
	   if(NULL=psid_priority_array->u.permissions_list.buf);
	   mbuf+=psid_priority->u.permissions_list.len*sizeof(u8);
	   size-=psid_priority->u.permissions_list.len*sizeof(u8);
	   return len-size;
	   break; */
	 default:
       bitnum=head_bit_num(mbuf);
	   psid_priority_array->u.other_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_priority_array->u.other_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_priority_array->u.other_permissions.buf=(u8*)malloc(sizeof(u8)*psid_priority->u.other_permissions.len);
	   if(NULL=psid_priority_array->u.other_permissions.buf)
		   return -1;
	   mbuf+=psid_priority_array->u.other_permissions.len*sizeof(u8);
	   size-=psid_priority_array->u.other_permissions.len*sizeof(u8);
	   return len-size;
	   break;
   }
}

//buf_2 15

static u32 buf_2_psid_array(const u8* buf,const u32 len,psid_array* psid_array){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

//	if(size<  )
		return -1;

	psid_array->type=get8(mbuf);
	mbuf++;
	size--;
  
   switch(psid_array->type){
	case ARRAY_TYPE_SPECIFIED:
	   //zheng shu lian 
	case ARRAY_TYPE_FROM_ISSUER:
	   break;
	default:
       bitnum=head_bit_num(mbuf);
	   psid_array->u.other_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_array->u.other_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_array->u.other_permissions.buf=(u8*)malloc(sizeof(u8)*psid_array->u.other_permissions.len);
	   if(NULL=psid_array->u.other_permissions.buf)
		   return -1;
	   mbuf+=psid_array->u.other_permissions.len*sizeof(u8);
	   size-=psid_array->u.other_permissions.len*sizeof(u8);
	   return len-size;
	   break;
   }
}

    
//buf_2 16

static u32 buf_2_psid_ssp(const u8* buf,const u32 len,psid_ssp* psid_ssp){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

//	if(size< )
		return -1;
  //psid  notunderstand
     bitnum=head_bit_num(mbuf);
	   psid_ssp->service_specific_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_ssp->service_specific_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_ssp->service_specific_permissions.buf=(u8*)malloc(sizeof(u8)*psid_ssp->service_specific_permissions.len);
	   if(NULL=psid_ssp->service_specific_permissions.buf)
		   return -1;
	   mbuf+=psid_ssp->service_specific_permissions.len*sizeof(u8);
	   size-=psid_array->service_specific_permissions.len*sizeof(u8);
	   return len-size;
}


//buf_2 17
static u32 buf_2_psid_ssp_array(const u8* buf,const u32 len,psid_ssp_array* psid_ssp_array){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

//	if(size<  )
		return -1;

	psid_ssp_array->type=get8(mbuf);
	mbuf++;
	size--;
  
   switch(psid_ssp_array->type){
	case ARRAY_TYPE_SPECIFIED:
	   //zheng shu lian 
	case ARRAY_TYPE_FROM_ISSUER:
	   break;
	default:
       bitnum=head_bit_num(mbuf);
	   psid_ssp_array->u.other_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_ssp_array->u.other_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_ssp_array->u.other_permissions.buf=(u8*)malloc(sizeof(u8)*psid_ssp_array->u.other_permissions.len);
	   if(NULL=psid_ssp_array->u.other_permissions.buf)
		   return -1;
	   mbuf+=psid_ssp_array->u.other_permissions.len*sizeof(u8);
	   size-=psid_ssp_array->u.other_permissions.len*sizeof(u8);
	   return len-size;
	   break;
   }
}

//buf_2 18
static u32 buf_2_psid_priority_ssp(const u8* buf,const u32 len,psid_priority_ssp* psid_priority_ssp){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
    //if(size<  )
		return -1;
   //psid  notunderstand
  psid_priority_ssp->max_priority=get8(mbuf);
	mbuf++
	size--;
       bitnum=head_bit_num(mbuf);
	   psid_priority_ssp->service_specific_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_priority_ssp->service_specific_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_priority_ssp->service_specific_permissions.buf=(u8*)malloc(sizeof(u8)*psid_priority_ssp->service_specific_permissions.len);
	   if(NULL=psid_priority_ssp->service_specific_permissions.buf)
		   return -1;
	   mbuf+=psid_priority_ssp->service_specific_permissions.len*sizeof(u8);
	   size-=psid_priority_ssp->service_specific_permissions.len*sizeof(u8);
	   return len-size;
}

//buf_2 19

static u32 buf_2_psid_priority_ssp_array(const u8* buf,const u32 len,psid_priority_ssp_array* psid_priority_ssp_array){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

//	if(size<  )
		return -1;
  psid_priority_ssp_array->type=get8(mbuf);
	mbuf++
	size--;
   switch(psid_priority_ssp_array->type){
	case ARRAY_TYPE_SPECIFIED:
	   //zheng shu lian 
	case ARRAY_TYPE_FROM_ISSUER:
	   break;
	default:
       bitnum=head_bit_num(mbuf);
	   psid_priority_ssp_array->u.other_permissions.len=variablelength_data_num(mbuf,bitnum);
	   if(size<psid_priority_ssp_array->u.other_permissions.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   psid_priority_ssp_array->u.other_permissions.buf=(u8*)malloc(sizeof(u8)*psid_priority_ssp_array->u.other_permissions.len);
	   if(NULL=psid_priority_ssp_array->u.other_permissions.buf)
		   return -1;
	   mbuf+=psid_priority_ssp_array->u.other_permissions.len*sizeof(u8);
	   size-=psid_priority_ssp_array->u.other_permissions.len*sizeof(u8);
	   return len-size;
	   break;
   }
}

//buf_to 20

static u32 buf_2_wsa_scope(const u8* buf,const u32 len,wsa_scope* wsa_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

	u32 geographic_length;
	u32 psid_priority_ssp_array_length;
	//if(size< )
   //	return -1;
	//u8* name;
	psid_priority_ssp_array_length=buf_2_psid_priority_ssp_array(mbuf,len,&wsa_scope->permissions);
      if(0==psid_priority_ssp_array_length)
		  return -1;
	  mbuf+=psid_priority_ssp_array_length;
	  size-=psid_priority_ssp_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&wsa_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}

//buf_2 21
static u32 buf_2_anonymous_scope(const u8* buf,const u32 len,anonymous_scope* anonymous_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

	u32 geographic_length;
	u32 psid_ssp_array_length;
	//if(size< )  return -1;
       bitnum=head_bit_num(mbuf);
	   anonymous_scope->additionla_data.len=variablelength_data_num(mbuf,bitnum);
	   if(size<anonymous_scope->additionla_data.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   anonymous_scope->additionla_data.buf=(u8*)malloc(sizeof(u8)*anonymous_scope->additionla_data.len);
	   if(NULL=anonymous_scope->additionla_data.buf)
		   return -1;
	   mbuf+=anonymous_scope->additionla_data.len*sizeof(u8);
	   size-=anonymous_scope->additionla_data.len*sizeof(u8);
	   return len-size;

	u32 geographic_length;
	u32 psid_ssp_array_length;
	//if(size< )
   //	return -1;
	psid_ssp_array_length=buf_2_psid_ssp_array(mbuf,len,&anonymous_scope->permissions);
      if(0==psid_ssp_array_length)
		  return -1;
	  mbuf+=psid_ssp_array_length;
	  size-=psid_ssp_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&anonymous_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}

//buf_2 22
static u32 buf_2_identified_scope(const u8* buf,const u32 len,identified_scope* identified_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

	u32 geographic_length;
	u32 psid_ssp_array_length;
	//if(size< )
   //	return -1;
	//u8* name;
	psid_ssp_array_length=buf_2_psid_ssp_array(mbuf,len,&identified_scope->permissions);
      if(0==psid_ssp_array_length)
		  return -1;
	  mbuf+=psid_ssp_array_length;
	  size-=psid_ssp_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&identified_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}

//buf_2 23

static u32 buf_2_identified_not_localized_scope(const u8* buf,const u32 len,identified_not_localized_scope* identified_not_localized_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
	u32 psid_ssp_array_length;
	//if(size< )
   //	return -1;
	//u8*
	psid_ssp_array_length=buf_2_psid_ssp_array(mbuf,len,&identified_not_localized_scope->permissions);
      if(0==psid_ssp_array_length)
		  return -1;
	  mbuf+=psid_ssp_array_length;
	  size-=psid_ssp_array_length;
	  return len-size;
}

//buf_2 24
static u32 buf_2_wsa_ca_scope(const u8* buf,const u32 len,wsa_ca_scope* wsa_ca_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

	u32 geographic_length;
	u32 psid_priority_array_length;
	//if(size< )  return -1;
       bitnum=head_bit_num(mbuf);
	   wsa_ca_scope->name.len=variablelength_data_num(mbuf,bitnum);
	   if(size<wsa_ca_scope->name.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   wsa_ca_scope->name.buf=(u8*)malloc(sizeof(u8)*wsa_ca_scope->name.len);
	   if(NULL=wsa_ca_scope->name.buf)
		   return -1;
	   mbuf+=wsa_ca_scope->name.len*sizeof(u8);
	   size-=wsa_ca_scope->name.len*sizeof(u8);
	   return len-size;

	//if(size< )
   //	return -1;
	psid_priority_array_length=buf_2_psid_priority_array(mbuf,len,&wsa_ca_scope->permissions);
      if(0==psid_priority_array_length)
		  return -1;
	  mbuf+=psid_priority_array_length;
	  size-=psid_priority_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&wsa_ca_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}

//buf_2 25
static u32 buf_2_sec_data_exch_ca_scope(const u8* buf,const u32 len,sec_data_exch_ca_scope* sec_data_exch_ca_scope){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;

	u32 geographic_length;
	u32 psid_array_length;
	//if(size< )  return -1;
       bitnum=head_bit_num(mbuf);
	  sec_data_exch_ca_scope ->name.len=variablelength_data_num(mbuf,bitnum);
	   if(size<sec_data_exch_ca_scope->name.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   sec_data_exch_ca_scope->name.buf=(u8*)malloc(sizeof(u8)*sec_data_exch_ca_scope->name.len);
	   if(NULL=sec_data_exch_ca_scope->name.buf)
		   return -1;
	   mbuf+=sec_data_exch_ca_scope->name.len*sizeof(u8);
	   size-=sec_data_exch_ca_scope->name.len*sizeof(u8);
	   return len-size;

	   //holder_type_flags

	//if(size< )
   //	return -1;
	psid_array_length=buf_2_psid_array(mbuf,len,&sec_data_exch_ca_scope->permissions);
      if(0==psid_array_length)
		  return -1;
	  mbuf+=psid_array_length;
	  size-=psid_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&sec_data_exch_ca_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}

//buf_2 26
static u32 buf_2_root_ca_scope(const u8* buf,const u32 len,root_ca_scope* root_ca_scope){
	u8* mbuf=buf;
	u16 bitnum1;
	u16 bitnum2;
	u32 size=len;

	u32 geographic_length;
	u32 psid_array_length;
	u32 psid_priority_array_length;

	//if(size< ) return -1;
    bitnum1=head_bit_num(mbuf);
	  root_ca_scope->name.len=variablelength_data_num(mbuf,bitnum);
	   if(size<root_ca_scope->name.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   root_ca_scope->name.buf=(u8*)malloc(sizeof(u8)*root_ca_scope->name.len);
	   if(NULL=root_ca_scope->name.buf)
		   return -1;
	   mbuf+=root_ca_scope->name.len*sizeof(u8);
	   size-=root_ca_scope->name.len*sizeof(u8);
	   return len-size;
     //holder_type_flags

	   psid_array_length=buf_2_psid_array(mbuf,len,&root_ca_scope->flags_content.secure_data_permissions);
      if(0==psid_array_length)
		  return -1;
	  mbuf+=psid_array_length;
	  size-=psid_array_length;
	  return len-size;

	psid_priority_array_length=buf_2_psid_priority_array(mbuf,len,&root_ca_scope
			->flags_content.wsa_permissions);
      if(0==psid_priority_array_length)
		  return -1;
	  mbuf+=psid_priority_array_length;
	  size-=psid_priority_array_length;
	  return len-size;

	geographic_length=buf_2_geographic_region(mbuf,len,&root_ca_scope->region);
      if(0==geographic_length)
		  return -1;
	  mbuf+=geographic_length;
	  size-=geographic_length;
	  return len-size;
}


//buf_2 27
static u32 buf_2_cert_specific_data(const u8* buf,const u32 len,cert_specific_data* cert_specific_data, holder_type holder_type ){
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
	u32 root_ca_scope_length;
	u32 sec_data_exch_ca_scope_length;
	u32 wsa_ca_scope_length;
	u32 identified_not_localized_scope_length;
	u32 identified_scope_length;
	u32 anonymous_scope_length;
    u32 wsa_scope_length;

	//if(size< )
   //	return -1;
  switch(holder_type){
		case ROOT_CA:
	root_ca_scope_length=buf_2_root_ca_scope(mbuf,len,&cert_specific_data->u.root_ca_scope)
		 if(0==root_ca_scope_length)
		  return -1;
	  mbuf+=root_ca_scope_length;
	  size-=root_ca_scope_length;
	  return len-size;
	  break;
		case SDE_CA:
	    case SDE_ENROLMENT:
	  sec_data_exch_ca_scope_length=buf_2_sec_data_exch_ca_scope(mbuf,len,&cert_specific_data ->u.sde_ca_scope);
     if(0==sec_data_exch_ca_scope_length)
		  return -1;
	  mbuf+=sec_data_exch_ca_scope_length;
	  size-=sec_data_exch_ca_scope_length;
	  return len-size;
	  break;
		case WSA_CA:
	    case WSA_ENROLMENT:
      wsa_ca_scope_length=buf_2_wsa_ca_scope(mbuf,len,&cert_specific_data->u.wsa_ca_scope);
     if(0==wsa_ca_scope_length)
		  return -1;
	  mbuf+=wsa_ca_scope_length;
	  size-=wsa_ca_scope_length;
	  return len-size;
	  break;
     case CRL_SIGNER:
  //crl_scs*
	 case SDE_IDENTIFIED_NOT_LOCALIZED:
    identified_not_localized_scope_length= buf_2_identified_not_localized_scope(mbuf,len,&cert_specific_data->u.id_non_loc_scope);
    if(0==identified_not_localized_scope_length)
		  return -1;
	  mbuf+=identified_not_localized_scope_length;
	  size-=identified_not_localized_scope_length;
	  return len-size;
	  break;
	 case SDE_IDENTIFIED_LOCALIZED:
	  identified_scope_length=buf_2_identified_scope(mbuf,len,&cert_specific_data->u.id_scope);
      if(0==identified_scope_length)
		  return -1;
	  mbuf+=identified_scope_length;
	  size-=identified_scope_length;
	  return len-size;
	  break;
	 case SDE_ANONYMOUS:
	  anonymous_scope_length=buf_2_anonymous_scope(mbuf,len,&cert_specific_data->u.anonymous_scope);
     if(0==anonymous_scope_length)
		  return -1;
	  mbuf+=anonymous_scope_length;
	  size-=anonymous_scope_length;
	  return len-size;
	  break;
	 case WSA:
	  wsa_scope_length=buf_2_wsa_scope(mbuf,len,&cert_specific_data->u.wsa_scope);
      if(0==wsa_scope_length)
		  return -1;
	  mbuf+=wsa_scope_length;
	  size-=wsa_scope_length;
	  return len-size;
      break;
	  default:
       bitnum=head_bit_num(mbuf);
	  cert_specific_data->u.other_scope.len=variablelength_data_num(mbuf,bitnum);
	   if(size<cert_specific_data->u.other_scope.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   cert_specific_data->u.other_scope.buf=(u8*)malloc(sizeof(u8)*cert_specific_data->u.other_scope.len);
	   if(NULL=cert_specific_data->u.other_scope.buf)
		   return -1;
	   mbuf+=cert_specific_data->u.other_scope.len*sizeof(u8);
	   size-=cert_specific_data->u.other_scope.len*sizeof(u8);
	   return len-size;
}

}


//buf_2 28
static u32 buf_2_tobesigned_certificate(const u8* buf,const u32 len,tobesigned_certificate* tobesigned_certificate,u8 version_and_type ){
	u8* mbuf=buf;
	u16 bitnum;
	u16 bitnum2;
	u32 size=len;
	u32 cert_specific_data_length;
	u32 public_key_length;
	u32 public_key_length2;

	//if(size< )  return -1;
   tobesigned_certificate->holder_type=get8(mbuf);
	mbuf++
	size--;
//  content flag
	switch(tobesigned_certificate->holder_type){
		case ROOT_CA:
			break;
	     default:
//    hashedid8 
	  tobesigned_certificate->u.no_root_ca.signature_alg=get8(mbuf);
	mbuf++
	size--;
	break;
	}

   cert_specific_data_length=buf_2_cert_specific_data(mbuf,len,&tobesigned_certificate->scope);
      if(0==cert_specific_data_length)
		  return -1;
	  mbuf+=cert_specific_data_length;
	  size-=cert_specific_data_length;
	
	  tobesigned_certificate->expiration=get32(mbuf);
      tobesigned_certificate->expiration=be_to_host32(tobesigned_certificate->expiration);
     mbuf+=4;
     size-=4;
    
 tobesigned_certificate->crl_series=get32(mbuf);
      tobesigned_certificate->crl_series=be_to_host32(tobesigned_certificate->crl_series);
     mbuf+=4;
     size-=4;

  switch(version_and_type){
	  case 2:
       public_key_length=buf_2_public_key(mbuf,len,&tobesigned_certificate->version_and_type.verification_key);
      if(0==public_key_length)
		  return -1;
	  mbuf+=public_key_length;
	  size-=public_key_length;
	  return len-size;
	  
	  case 3:
	  break;

	  default:
	  bitnum=head_bit_num(mbuf);
	  tobesigned_certificate->version_and_type.other_key_material.len=variablelength_data_num(mbuf,bitnum);
	   if(size< tobesigned_certificate->version_and_type.other_key_material.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	    tobesigned_certificate->version_and_type.other_key_material.buf=(u8*)malloc(sizeof(u8)* tobesigned_certificate->version_and_type.other_key_material.len);
	   if(NULL= tobesigned_certificate->version_and_type.other_key_material.buf)
		   return -1;
	   mbuf+= tobesigned_certificate->version_and_type.other_key_material.len*sizeof(u8);
	   size-= tobesigned_certificate->version_and_type.other_key_material.len*sizeof(u8);
	   return len-size;
	   break;
  }
	 //certification_duration   meiyou
      tobesigned_certificate->flags_content.start_validity=get32(mbuf);
      tobesigned_certificate->flags_content.start_validity=be_to_host32(tobesigned_certificate->flags_content.start_validity);
     mbuf+=4;
     size-=4;

 public_key_length2=buf_2_public_key(mbuf,len,&tobesigned_certificate->flags_content.encryption_key);
      if(0==public_key_length2)
		  return -1;
	  mbuf+=public_key_length2;
	  size-=public_key_length2;
	  
      bitnum2=head_bit_num(mbuf);
	  tobesigned_certificate->flags_content.other_cert_content.len=variablelength_data_num(mbuf,bitnum);
	   if(size< tobesigned_certificate->.flags_content.other_cert_content.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	    tobesigned_certificate->flags_content.other_cert_content.buf=(u8*)malloc(sizeof(u8)* tobesigned_certificate->flags_content.other_cert_content.len);
	   if(NULL= tobesigned_certificate->flags_content.other_cert_content.buf)
		   return -1;
	   mbuf+= tobesigned_certificate->flags_content.other_cert_content.len*sizeof(u8);
	   size-= tobesigned_certificate->flags_content.other_cert_content.len*sizeof(u8);
	   return len-size;
}

//buf_2 29
static u32 buf_2_certificate(const u8* buf,const u32 len,certificate* certificate){
  
	u8* mbuf=buf;
	u16 bitnum;
	u32 size=len;
	u32 tobesigned_certificate_length;
    u32 signature_length;
	u32 elliptic_curve_point_length;
	//if(size< ) return -1;
	certificate->version_and_type=get8(mbuf);
	mbuf++
	size--;
  
	tobesigned_certificate_length=buf_2_tobesigned_certificate(mbuf,len,&certificate->unsigned_certificate);
      if(0==tobesigned_certificate_length)
		  return -1;
	  mbuf+=tobesigned_certificate_length;
	  size-=tobesigned_certificate_length;

	 switch(certificate->version_and_type){
		 case 2:
	 signature_length=buf_2_signature(mbuf,len,&certificate->u.signature);
      if(0==signature_length)
		  return -1;
	  mbuf+=signature_length;
	  size-=signature_length;
	  return len-size;
	  break;  
	 case 3:
	 elliptic_curve_point_length=buf_2_elliptic_curve_point(mbuf,len,&certificate->u.reconstruction_value);
      if(0==elliptic_curve_point_length)
		  return -1;
	  mbuf+=elliptic_curve_point_length;
	  size-=elliptic_curve_point_length;
	  break;
	  return len-size;
	 default:
      bitnum=head_bit_num(mbuf);
	  certificate->u.signature_material.len=variablelength_data_num(mbuf,bitnum);
	   if(size< certificate->u.signature_material.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   certificate->u.signature_material.buf=(u8*)malloc(sizeof(u8)* certificate->u.signature_material.len);
	   if(NULL=  certificate->u.signature_material.buf)
		   return -1;
	   mbuf+=certificate->u.signature_material.len*sizeof(u8);
	   size-=certificate->u.signature_material.len*sizeof(u8);
	   return len-size;
	 }
}


//buf_2 30
static u32 buf_2_signer_identifier(const u8* buf,const u32 len,signer_identifier* signer_identifier){
 
	u8* mbuf=buf;
	u16 bitnum;
	u16 bitnum2;
	u32 size=len;
	u32 certificate_length;
    
	//if(size< )  return -1;
	signer_identifier->type=get8(mbuf);
	mbuf++;
	size--;

	switch(signer_identifier->type){
		case SELF:
			break;
		case CERTIFICATE_DIGEST_WITH_ECDSAP224:
		case CERTIFICATE_DIGEST_WITH_ECDSAP256:
		//hashedid8	
		case CERTIFICATE:
	      certificate_length=buf_2_certificate(mbuf,len,&signer_identifier->u.certificate);
      if(0==certificate_length)
		  return -1;
	  mbuf+=certificate_length;
	  size-=certificate_length;
	  return len-size;
	  break;
		case CERTIFICATE_CHAIN:
        bitnum=head_bit_num(mbuf);
	   signer_identifier->u.certificates.len=variablelength_data_num(mbuf,bitnum);
	   if(size< signer_identifier->u.certificates.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   signer_identifier->u.certificates.buf=(u8*)malloc(sizeof(u8)*signer_identifier->u.certificates.len);
	   if(NULL==signer_identifier->u.certificates.buf)
		   return -1;
	   mbuf+=signer_identifier->u.certificates.len*sizeof(u8);
	   size-=signer_identifier->u.certificates.len*sizeof(u8);
	   return len-size;
	   break;
       case CERTIFICATE_DIGETS_WITH_OTHER_ALGORITHM:
    	signer_identifier->u.other_algorithm.algorithm=get8(mbuf);
    	mbuf++;
     	size--;
		//hashedid8
		return len-size;
		break;
	   default:
    	 bitnum2=head_bit_num(mbuf);
	   signer_identifier->u.id.len=variablelength_data_num(mbuf,bitnum);
	   if(size< signer_identifier->u.id.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   signer_identifier->u.id.buf=(u8*)malloc(sizeof(u8)*signer_identifier->u.id.len);
	   if(NULL==signer_identifier->u.id.buf)
		   return -1;
	   mbuf+=signer_identifier->u.id.len*sizeof(u8);
	   size-=signer_identifier->u.id.len*sizeof(u8);
	   return len-size;
	   break;
	}
}

  
// buf_2 31
static u32 buf_2_crl_request(const u8* buf,const u32 len,crl_request* crl_request){
	u8* mbuf=buf;
	u32 size=len;

  //  if(size< ) return -1;	
	//hashid8
	crl_request->crl_series= get32(mbuf);
	be_to_host32(crl_request->crl_series);
	mbuf+=4;
	size-=4;

	crl_request->issue_date= get32(mbuf);
	be_to_host32(crl_request->issue_date);
	mbuf+=4;
	size-=4;

	return len-size;
}


// buf_2 32
static u32 buf_2_certid10(const u8* buf, const u32 len, certid10* certid10){
	u8* mbuf = buf; 
	u32 size = len;
	int i;
	if (size < 10)
		return 0;
	for(i = 0; i < 10; i++ ){
	    certid10->certid10[i] = get8(mbuf);
		mbuf++;
		size--
	}
	return len-size;
}


//buf_2 33
static u32 buf_2_id_and_date(const u8* buf, const u32 len, id_and_date* id_and_date){
   	u8* mbuf = buf; 
	u32 size = len;
	u32 certid10_length;

    certid10_length=buf_2_certid10(mbuf,len,&id_and_date->id);
      if(0==certid10_length)
		  return -1;
	  mbuf+=certid10_length;
	  size-=certid10_length;

	id_and_date->expiry= get32(mbuf);
	be_to_host32(id_and_date->expiry);
	mbuf+=4;
	size-=4;
    return len-size;
}


// buf_2 34
static u32 buf_2_tobesigned_crl(const u8* buf, const u32 len, tobesigned_crl* tobesigned_crl){
   	u8* mbuf = buf; 
	u32 size = len;
    u16 bitnum;
//  if(size< )  return -1;
	tobesigned_crl->type=get8(mbuf);
	mbuf++;
	size--;

	tobesigned_crl->crl_series= get32(mbuf);
	be_to_host32(tobesigned_crl->crl_series);
	mbuf+=4;
	size-=4;

 //harshid8
	tobesigned_crl->crl_serial= get32(mbuf);
	be_to_host32(tobesigned_crl->crl_serial);
	mbuf+=4;
	size-=4;

	tobesigned_crl->start_period= get32(mbuf);
	be_to_host32(tobesigned_crl->start_period);
	mbuf+=4;
	size-=4;

	tobesigned_crl->issue_date= get32(mbuf);
	be_to_host32(tobesigned_crl->issue_date);
	mbuf+=4;
	size-=4;

	tobesigned_crl->next_crl= get32(mbuf);
	be_to_host32(tobesigned_crl->next_crl);
	mbuf+=4;
	size-=4;

	 switch(tobesigned_crl->type){
		 case ID_ONLY:
	   bitnum=head_bit_num(mbuf);
	   tobesigned_crl->u.entries.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_crl->u.entries.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_crl->u.entries.buf=(u8*)malloc(sizeof(u8)*tobesigned_crl->u.entries.len);
	   if(NULL== tobesigned_crl->u.entries.buf)
		   return -1;
	   mbuf+= tobesigned_crl->u.entries.len*sizeof(u8);
	   size-= tobesigned_crl->u.entries.len*sizeof(u8);
	   return len-size;
	   break;
	
		 case ID_AND_EXPIRY:
/*	    bitnum=head_bit_num(mbuf);
	   tobesigned_crl->u.entries.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_crl->u.entries.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_crl->u.entries.buf=(u8*)malloc(sizeof(u8)*tobesigned_crl->u.entries.len);
	   if(NULL== tobesigned_crl->u.entries.buf)
		   return -1;
	   mbuf+= tobesigned_crl->u.entries.len*sizeof(u8);
	   size-= tobesigned_crl->u.entries.len*sizeof(u8);
	   return len-size;
	   break;
	   */
		 default:
       bitnum=head_bit_num(mbuf);
	   tobesigned_crl->u.other_entries.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_crl->u.other_entries.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_crl->u.other_entries.buf=(u8*)malloc(sizeof(u8)*tobesigned_crl->u.other_entries.len);
	   if(NULL== tobesigned_crl->u.other_entries.buf)
		   return -1;
	   mbuf+= tobesigned_crl->u.other_entries.len*sizeof(u8);
	   size-= tobesigned_crl->u.other_entries.len*sizeof(u8);
	   return len-size;
	   break;
	 }
}


//buf_2 35
static u32 buf_2_crl(const u8* buf, const u32 len, crl* crl){
   	u8* mbuf = buf; 
	u32 size = len;
    u32 signer_length;
	u32 tobesigned_length;
	u32 signature_length;
	
	crl->version=get8(mbuf);
	mbuf++;
	size--;

    signer_length=buf_2_signer_identifier(mbuf,len,&crl->version);
      if(0==signer_length)
		  return -1;
	  mbuf+=signer_length;
	  size-=signer_length;

    tobesigned_length=buf_2_tobesigned_crl(mbuf,len,&crl->unsigned_crl);
      if(0==tobesigned_length)
		  return -1;
	  mbuf+=tobesigned_length;
	  size-=tobesigned_length;

    signature_length=buf_2_signature(mbuf,len,&crl->signature);
      if(0==signature_length)
		  return -1;
	  mbuf+=signature_length;
	  size-=signature_length;
 }

//buf_2 36
static u32 buf_2_tobe_encrypted_certificate_response_acknowledgment(const u8* buf, const u32 len, tobe_encrypted_certificate_response_acknowledgment* tobe_encrypted_certificate_response_acknowledgment){
	u8* mbuf = buf; 
	u32 size = len;
	int i;
	if (size < 10)
		return 0;
	for(i = 0; i < 10; i++ ){
	    tobe_encrypted_certificate_response_acknowledgment->response_hash[i] = get8(mbuf);
		mbuf++;
		size--
	}
	return len-size;
}

//buf_2 37
static u32 buf_2_tobe_encrypted_certificate_request_error(const u8* buf, const u32 len, tobe_encrypted_certificate_request_error* tobe_encrypted_certificate_request_error){
	u8* mbuf = buf; 
	u32 size = len;_
	u32 signer_length;
	u32 certificate_length;
	u32 signature_length;

	//if(size<)  return -1;
    signer_length=buf_2_signer_identifier(mbuf,len,&tobe_encrypted_certificate_request_error->signer);
      if(0==signer_length)
		  return -1;
	  mbuf+=signer_length;
	  size-=signer_length;

	for(i = 0; i < 8; i++ ){
	    tobe_encrypted_certificate_request_error->request_hash[i] = get8(mbuf);
		mbuf++;
		size--
	}

    certificate_length=buf_2_certificate_request_error_code(mbuf,len,&tobe_encrypted_certificate_request_error->reason);
      if(0==certificate_length)
		  return -1;
	  mbuf+=certificate_length;
	  size-=certificate_length;

    signature_length=buf_2_signature(mbuf,len,&tobe_encrypted_certificate_request_error->signature);
      if(0==signature_length)
		  return -1;
	  mbuf+=signature_length;
	  size-=signature_length;
	  
	  return len-size;
}

//buf_2 38
static u32 buf_2_  tobe_encrypted_certificate_response(const u8* buf, const u32 len, 
		tobe_encrypted_certificate_response* tobe_encrypted_certificate_response,u8 version_and_type){
   	u8* mbuf = buf; 
	u32 size = len;
	u16 bitnum;

	//if(size< ) return -1;
	tobe_encrypted_certificate_response->f=get8(mbuf);
	mbuf++;
	size--;
  
	switch(version_and_type){
		case 2:
			break;
		case 3:
       bitnum=head_bit_num(mbuf);
	   tobe_encrypted_certificate_response->u.recon_priv.len=variablelength_data_num(mbuf,bitnum);
	   if(tobe_encrypted_certificate_response->u.recon_priv.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	tobe_encrypted_certificate_response->u.recon_priv.buf=(u8*)malloc(sizeof(u8)*tobe_encrypted_certificate_response->u.recon_priv.len);
	   if(NULL== tobe_encrypted_certificate_response->u.recon_priv.buf)
		   return -1;
	   mbuf+= tobe_encrypted_certificate_response->u.recon_priv.len*sizeof(u8);
	   size-= tobe_encrypted_certificate_response->u.recon_priv.len*sizeof(u8);
	   return len-size;
	   break;
		default:
     bitnum=head_bit_num(mbuf);
	   tobe_encrypted_certificate_response->u.other_material.len=variablelength_data_num(mbuf,bitnum);
	   if(tobe_encrypted_certificate_response->u.other_material.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	tobe_encrypted_certificate_response->u.other_material.buf=(u8*)malloc(sizeof(u8)*tobe_encrypted_certificate_response->u.other_material.len);
	   if(NULL== tobe_encrypted_certificate_response->u.other_material.buf)
		   return -1;
	   mbuf+= tobe_encrypted_certificate_response->u.other_material.len*sizeof(u8);
	   size-= tobe_encrypted_certificate_response->u.other_material.len*sizeof(u8);
	   return len-size;
	   break;
	}
    //zhengshulian
}


//buf_2 39
static u32 buf_2_tobesigned_certificate_request(const u8* buf, const u32 len, tobesigned_certificate_request* tobesigned_certificate_request){
   	u8* mbuf = buf; 
	u32 size = len;
	u32 cert_specific_data_length;
	u32 public_key_length1;
	u32 public_key_length2;
	u32 public_key_length3;

    //if(size<)  return -1;
	tobesigned_certificate_request->version_and_type=get8(mbuf);
	mbuf++;
	size--;

	tobesigned_certificate_request->request_time= get32(mbuf);
	be_to_host32(tobesigned_certificate_request->request_time);
	mbuf+=4;
	size-=4;
    
	tobesigned_certificate_request->holder_type=get8(mbuf);
	mbuf++;
	size--;
    
  //flag
	cert_specific_data_length=buf_2_cert_specific_data(mbuf,len,&tobesigned_certificate_request->signature);
      if(0==cert_specific_data_length)
		  return -1;
	  mbuf+=cert_specific_data_length;
	  size-=cert_specific_data_length;

	tobesigned_certificate_request->expiration= get32(mbuf);
	be_to_host32(tobesigned_certificate_request->expiration);
	mbuf+=4;
	size-=4;

	tobesigned_certificate_request->flags_content.lifetime= get16(mbuf);
	be_to_host16(tobesigned_certificate_request->flags_content.liftime);
	mbuf+=2;
	size-=2;

	tobesigned_certificate_request->flags_content.start_validity= get32(mbuf);
	be_to_host32(tobesigned_certificate_request->flags_content.start_validity);
	mbuf+=4;
	size-=4;

	public_key_length1=buf_2_public_key(mbuf,len,&tobesigned_certificate_request->flags_content.encrypted_data);
      if(0==public_key_length1)
		  return -1;
	  mbuf+=public_key_length1;
	  size-=public_key_length1;

	public_key_length2=buf_2_public_key(mbuf,len,&tobesigned_certificate_request->verification_key);
      if(0==public_key_length2)
		  return -1;
	  mbuf+=public_key_length2;
	  size-=public_key_length2;

	public_key_length3=buf_2_public_key(mbuf,len,&tobesigned_certificate_request->response_encryption_key);
      if(0==public_key_length3)
		  return -1;
	  mbuf+=public_key_length3;
	  size-=public_key_length3;

	  return len-size;
}

//buf_2 40
static u32 buf_2_certificate_request(const u8* buf, const u32 len, certificate_request* certificate_request){
   	u8* mbuf = buf; 
	u32 size = len;	  
	u32 signer_length;
	u32 tobesigned_length;
	u32 signature_length;
    signer_length=buf_2_signer_identifier(mbuf,len,&certificate_request->signer);
      if(0==signer_length)
		  return -1;
	  mbuf+=signer_length;
	 size-=signer_length;

  tobesigned_length=buf_2_tobesigned_certificate_request(mbuf,len,&certificate_request->unsigned_csr);
      if(0==tobesigned_length)
		  return -1;
	  mbuf+=tobesigned_length;
	 size-=tobesigned_length;

   signature_length=buf_2_signature(mbuf,len,&certificate_request->signature);
      if(0==signature_length)
		  return -1;
	  mbuf+=signature_length;
	 size-=signature_length;
}

//buf_2 41
static u32 buf_2_tobesigned_data(const u8* buf, const u32 len, tobesigned_data* tobesigned_data,content_type type){
   	u8* mbuf = buf; 
	u32 size = len;	  
    u16 bitnum;
	u16 bitnum2;
	u16 bitnum3;
	u32 time64_length;
	u32 three_length;
	//if(size< )  return -1
	//flag
	switch(type){
		case SIGNED:
   //psid
       bitnum=head_bit_num(mbuf);
	   tobesigned_data->u.type_signed.data.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_data->u.type_signed.data.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_data->u.type_signed.data.buf=(u8*)malloc(sizeof(u8)*.tobesigned_data->u.type_signed.data.len);
	   if(NULL== tobesigned_data->u.type_signed.data.buf)
		   return -1;
	   mbuf+= tobesigned_data->u.type_signed.data.len*sizeof(u8);
	   size-= tobesigned_data->u.type_signed.data.len*sizeof(u8);
	   return len-size;
	   break;
       case SIGNED_PARTIAL_PAYLOAD:
   //psid
       bitnum=head_bit_num(mbuf);
	   tobesigned_data->u.type_signed.data.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_data->u.type_signed_partical.data.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_data->u.type_signed_partical.data.buf=(u8*)malloc(sizeof(u8)*.tobesigned_data->u.type_signed_partical.data.len);
	   if(NULL== tobesigned_data->u.type_signed_partical.data.buf)
		   return -1;
	   mbuf+= tobesigned_data->u.type_signed_partical.data.len*sizeof(u8);
	   size-= tobesigned_data->u.type_signed_partical.data.len*sizeof(u8);
	   return len-size;
	   break;
	   case SIGNED_EXTERNAL_PAYLOAD:
	   //psid;
	   default:
       bitnum=head_bit_num(mbuf);
	   tobesigned_data->u.data.len=variablelength_data_num(mbuf,bitnum);
	   if(size<tobesigned_data->u.data.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobesigned_data->u.data.buf=(u8*)malloc(sizeof(u8)*.tobesigned_data->u.data.len);
	   if(NULL== tobesigned_data->u.data.buf)
		   return -1;
	   mbuf+= tobesigned_data->u.data.len*sizeof(u8);
	   size-= tobesigned_data->u.data.len*sizeof(u8);
	   return len-size;
	   break;
	}
  
     time64_length=buf_2_time64_with_standard_deviation(mbuf,len,&tobesigned_data->flags_content.generation_time);
      if(0==time64_length)
		  return -1;
	  mbuf+=time64_length;
	 size-=time64_length;

	tobesigned_data->flags_content.exipir_time= get64(mbuf);
	be_to_host64(tobesigned_data->flags_content.exipir_time);
	mbuf+=8;
	size-=8;

  three_length=buf_2_three_d_location(mbuf,len,&tobesigned_data->flags_content.generation_location);
      if(0==three_length)
		  return -1;
	  mbuf+=three_length;
	 size-=three_length;

    bitnum2=head_bit_num(mbuf);
	   tobesigned_data->flags_content.extensions.len=variablelength_data_num(mbuf,bitnum2);
	   if(tobesigned_data->flags_content.extensions.len*sizeof(u8)+bitnum2)
		   return -1;
	   mbuf+=bitnum2;
	   size+=bitnum2;
	   tobesigned_data->flags_content.extensions.buf=(u8*)malloc(sizeof(u8)*tobesigned_data->flags_content.extensions.len);
	   if(NULL== tobesigned_data->flags_content.extensions.buf)
		   return -1;
	   mbuf+= tobesigned_data->flags_content.extensions.len*sizeof(u8);
	   size-= tobesigned_data->flags_content.extensions.len*sizeof(u8);
	   return len-size;
	   break;


    bitnum3=head_bit_num(mbuf);
	   tobesigned_data->flags_content.other_data.len=variablelength_data_num(mbuf,bitnum3);
	   if(tobesigned_data->flags_content.other_data.len*sizeof(u8)+bitnum3)
		   return -1;
	   mbuf+=bitnum3;
	   size+=bitnum3;
	   tobesigned_data->flags_content.other_data.buf=(u8*)malloc(sizeof(u8)*tobesigned_data->flags_content.other_data.len);
	   if(NULL== tobesigned_data->flags_content.other_data.buf)
		   return -1;
	   mbuf+= tobesigned_data->flags_content.other_data.len*sizeof(u8);
	   size-= tobesigned_data->flags_content.other_data.len*sizeof(u8);
	   return len-size;
	   break;
}

/**
 *	将buf中的字节流转换成一个signed_data结构体，同样可以认为这个结构体指针指向了
 *	 一个分配好了的内存
 * @buf:装有字节流的buf
 * @len:字节流的长度
 * @signed_data 需要填充的数据结构指针
 * @type 外部传入参数
 *  返回值：0 失败; 大于零 成功返回占用了多少字节
 */
static u32 buf_2_signed_data(const u8* buf, const u32 len, signed_data* signed_data, content_type type) {
	u8* mbuf = buf;
	u32 size = len;
							
	u32 lenth_signed_data;//signed_data的字节长度
	length_signed_data = buf_2_signer_identifier(mbuf, size, &signed_data->signer);
	if(NULL == length_signed_data)
			return 0;
	mbuf += length_signed_data;
	size -= length_signed_data;// 还没有写完哈
}

//buf_2 43
static u32 buf_2_tobe_encrypted(const u8* buf, const u32 len, tobe_encrypted* tobe_encrypted) {
	u8* mbuf = buf;
	u32 size = len;
	u16 bitnum;
	u32  signed_length;
	u32  response_length;
	u32  anon_response_length;
	u32 request_length;
	u32 crl_request_length;
	u32 crl_length;
	u32 tobe_encrypted_length;

//if(size< ) return -1;
	tobe_encrypted->type=get8(mbuf);
	mbuf++;
	size--;

	switch(tobe_encrypted->type){
		case UNSECURED:
       bitnum=head_bit_num(mbuf);
	   tobe_encrypted->u.plain_text.len=variablelength_data_num(mbuf,bitnum);
	   if( tobe_encrypted->u.plain_text.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobe_encrypted->u.plain_text.buf=(u8*)malloc(sizeof(u8)*tobe_encrypted->u.plain_text.len);
	   if(NULL==tobe_encrypted->u.plain_text.buf)
		   return -1;
	   mbuf+=tobe_encrypted->u.plain_text.len*sizeof(u8);
	   size-=tobe_encrypted->u.plain_text.len*sizeof(u8);
	   return len-size;
	   break;
	   
		case SIGNED:
		case SIGNED_EXTERNAL_PAYLOAD:
		case SIGNED_PARTIAL_PAYLOAD:
         signed_length=buf_2_signed_data(mbuf,len,&tobe_encrypted->U.signed_data);
      if(0==signed_length)
		  return -1;
	  mbuf+=signed_length;
	 size-=signed_length;
        return len-size;
		break;
		
		case CERTIFICATE_REQUEST:
        request_length=buf_2_certificate_request(mbuf,len,&tobe_encrypted->u.request);
      if(0==request_length)
		  return -1;
	  mbuf+=request_length;
	 size-=request_length;
        return len-size;
		break;
		
		case CERTIFICATE_RESPONSE:
        response_length=buf_2_tobe_encrypted_certifcate_response(mbuf,len,&tobe_encrypted->u.response);
      if(0==response_length)
		  return -1;
	  mbuf+=response_length;
	 size-=response_length;
        return len-size;
		break;
           
		case ANOYMOUS_CERTIFICATE_RESPONSE
       anon_response_length=buf_2_tobe_encrypted_anonymous_cert_response(mbuf,len,&tobe_encrypted->u.response);
      if(0==anon_response_length)
		  return -1;
	  mbuf+=anon_response_length;
	 size-=anon_response_length;
        return len-size;
		break;

		case CERTIFICATE_REQUSET_ERROR:
       request_length=buf_2_tobe_encrypted_certificate_request_error(mbuf,len,&tobe_encrypted->u.request_error);
      if(0==request_length)
		  return -1;
	  mbuf+=request_length;
	 size-=request_length;
        return len-size;
		break;

		case CRL_REQUEST:
		crl_request_length=buf_2_crl_request(mbuf,len,&tobe_encrypted->u.crl_reqest);
      if(0==crl_request_length)
		  return -1;
	  mbuf+=crl_request_length;
	 size-=crl_request_length;
        return len-size;
		break;

		case CRL:
	crl_length=buf_2_crl(mbuf,len,&tobe_encrypted->u.crl);
      if(0==crl_length)
		  return -1;
	  mbuf+=crl_length;
	 size-=crl_length;
        return len-size;
		break;

		case CERTIFACATE_RESPONSE_ACKNOWLEDGMENT:
	tobe_encrypted_length=buf_2_tobe_encrypted_certificate_response_acknowledgment(mbuf,len,&tobe_encrypted->u.ack);
      if(0==tobe_encrypted_length)
		  return -1;
	  mbuf+=tobe_encrypted_length;
	 size-=tobe_encrypted_length;
        return len-size;
		break;
        default:
       bitnum=head_bit_num(mbuf);
	   tobe_encrypted->u.data.len=variablelength_data_num(mbuf,bitnum);
	   if( tobe_encrypted->u.data.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	   tobe_encrypted->u.data.buf=(u8*)malloc(sizeof(u8)*tobe_encrypted->u.data.len);
	   if(NULL==tobe_encrypted->u.data.buf)
		   return -1;
	   mbuf+=tobe_encrypted->u.data.len*sizeof(u8);
	   size-=tobe_encrypted->u.data.len*sizeof(u8);
	   return len-size;
	   break;
	}
}

//buf_2 44
static u32 buf_2_aes_ccm_ciphertext(const u8* buf, const u32 len, aes_ccm_ciphertext* aes_ccm_ciphertext) {
	u8* mbuf = buf;
	u32 size = len;
	u16  bitnum;
//if(size< )  return -1;
	for(i = 0; i < 12; i++ ){
	    aes_ccm_ciphertext->nonce[i] = get8(mbuf);
		mbuf++;
		size--
	}
      bitnum=head_bit_num(mbuf);
	   aes_ccm_ciphertext->ccm_ciphertext.len=variablelength_data_num(mbuf,bitnum);
	   if(aes_ccm_ciphertext->ccm_ciphertext.len*sizeof(u8)+bitnum)
		   return -1;
	   mbuf+=bitnum;
	   size+=bitnum;
	    aes_ccm_ciphertext->ccm_ciphertext.buf=(u8*)malloc(sizeof(u8)*aes_ccm_ciphertext->ccm_ciphertext.len);
	   if(NULL==aes_ccm_ciphertext->ccm_ciphertext.buf)
		   return -1;
	   mbuf+=aes_ccm_ciphertext->ccm_ciphertext.len*sizeof(u8);
	   size-=aes_ccm_ciphertext->ccm_ciphertext.len*sizeof(u8);
	   return len-size;






  

	}
  



    


/**
 *	将buf里面的字节流，转化成一个sec_data结构体，对于这个结构体指针，
 *	 这个接口可以认为这个指针指向了一个分配好的内存
 *  @buf:装有字节流的buf
 *  @len:字节流的长度
 *  @sec:需要填充的数据结构
 *  return: -1：转换失败；0 ：转换成功
 */
u32 buf_2_sec_data(const u8* buf,const u32 len, sec_data* sec_data){
    u8* mbuf = buf;
    u32 size = len;
    //检查长度是否满足最低要求
    if(size < 4)
        return -1;
	//
    sec_data->protocol_version = get8(mbuf);   //填充协议版本
    mbuf = mbuf + 1;
    size = size - 1;
    if(sec_data->protocol_version != CURRETN_VERSION )
        return -1;							   
  
    sec_data->type = get8(mbuf);              //填充content_type
    mbuf = mbuf + 1;
    size = size - 1;
    
    switch(sec_data->type){
        case UNSECURED:
			//应该先用一字节编码,表明所用数据的字节长度，然后再分配内存
			sec_data->u.data.len = get8(mbuf);
			mbuf += 1;
            sec_data->u.data.buf = (u8*)malloc(sizeof(u8)*sec_data->u.data.len);//填充opaque
            if(sec_data->u.data.buf == NULL)
                return -1;

            sec_data->u.data.buf = mbuf;//单个数据的大小位u8，所以不需要大小端转换函数
            mbuf += u.data.len;			//已经结束了，mbuf指针没有必要再移位了
            return 0;
        case SIGNED:
        case SIGNED_PARTIAL_PAYLOAD:
        case SIGNED_EXTERNAL_PAYLOAD:
            if(!buf_2_signed_data(mbuf,size,&sec_data->u.signed_data, sec_data->type))//需要完成signdata函数填充，同样内存结构体内存已经分配好了
                return -1;
            return 0;
        case SIGNED_WSA:
            if(!buf_2_signed_wsa(mbuf,size,&sec_data->u.signed_wsa))//同上
                return -1;
            return 0;
        case ENCRYPTED:
            if(!buf_2_encrypted_data(mbuf,size,&sec_data->u.encrypted_data))//
                 return -1;
            return 0;
        case CONTENT_TYPE_CRL_REQUEST:
            if(!buf_2_crl_request(mbuf,size,&sec_data->u.crl_request))//
                 return -1;
            return 0;
        case CRL:
            if(!buf_2_crl(mbuf,size,&sec_data->u.crl))//
                 return -1;
            return 0;
        default:
            sec_data->u.other_data.len = get8(mbuf);
			mbuf++;
			sec_data->u.other_data.buf = (u8*)malloc(sizeof(u8)*sec_data->u.other_data.len);//
            if(sec_data->u.other_data.buf == NULL)
                 return -1;
            sec_data->u.other_data.buf = mbuf;
			mbuf += sec_data->u.other_data.len;
            //sec_data->u.other_data.len = size;

    }
    return 0;
}

