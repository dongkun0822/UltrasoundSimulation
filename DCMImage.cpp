#include "pch.h"
#include "DCMImage.h"

CDCMImage::CDCMImage()
{
	little_endian_specified = 0;
	m_pixel_success = 0;

	explicit_specified = 0;

	slope_and_intercept_specified = 0;
	number_of_frames_specified = 0;
	frame_time_specified = 0;
	frame_time_vector_specified = 0;
	photometric_specified = 0;
	window_center_specified = 0;
	window_width_specified = 0;

	PixelRepresentation = 0;
	PhotometricInterpretation = 0;
	memset(TransferSyntax, 0, 100);

	BitsAllocated = 0;
	BitsStored = 0;
	HighBit = 0;
	WindowCenter = 0;
	WindowWidth = 0;
	SamplesPerPixel = 0;
	MaxPixelValue = 0;
	MinPixelValue = 0;
	m_Slope = 0;
	m_Intercept = 0;

	Rows = 0;
	Colums = 0;
	ImageWidth = 0;
	ImageHeight = 0;
	m_SaveImageWidth = m_SaveImageHeight = 0;
	m_SavePixelSizeX = m_SavePixelSizeY = 0.;
	BoxWidth = 0;
	BoxHeight = 0;
	BoxXpos = 0;
	BoxYpos = 0;
	WatchXpos = 0;
	WatchYpos = 0;
	WatchWidth = 0;
	WatchHeight = 0;
	SltXpos = 0;
	SltYpos = 0;
	SltWidth = 0;
	SltHeight = 0;
	SltImgCenX = 0;
	SltImgCenY = 0;

	NumberOfFrames = 0;
	FrameTime = 0.;
	FrameTimeVector = 0;
	RepetitionTime = 0;
	EchoTime = 0;
	FrameDelay = 0.;

	memset(ImageNumber, 0, 100);
	memset(ImageDate, 0, 100);
	memset(ImageTime, 0, 100);
	memset(ImageType, 0, 100);

	memset(PatientName, 0, 100);
	memset(PatientID, 0, 100);
	memset(PatientAge, 0, 100);
	memset(PatientSex, 0, 100);


	memset(StudyID, 0, 100);
	memset(StudyTime, 0, 100);
	memset(StudyDate, 0, 100);


	memset(SeriesNumber, 0, 100);
	memset(SeriesDate, 0, 100);
	memset(SeriesTime, 0, 100);

	memset(AcquisitionDate, 0, 100);
	memset(AcquisitionTime, 0, 100);

	memset(Modality, 0, 100);
	memset(Manufacturer, 0, 100);
	memset(ManufacturerMode, 0, 100);
	memset(InstitutionAddress, 0, 100);
	memset(InstitutionName, 0, 100);

	memset(DFOV, 0, 100);
	memset(KVP, 0, 100);
	memset(TubeCurrent, 0, 100);
	memset(DetectorTilt, 0, 100);
	memset(BodyPart, 0, 100);
	memset(SliceLocation, 0, 100);
	memset(SliceThickness, 0, 100);

	memset(ImageOrientation, 0, 6 * sizeof(double));
	memset(OrientMark, 0, 4);
	memset(SaveOrientMark, 0, 4);
	memset(ImagePosition, 0, 3 * sizeof(double));

	PixelValueData = NULL;
	SavePixelValueData = NULL;
	WatchData = NULL;

	OW_specified = 0;
	Selected = 0;
	Deleted = 0;
	StateChanged = 0;
	BoxColor = RGB(0, 0, 0);
	DisplayMethod = 0;
	NoRegionX = NoRegionY = NoRegionWidth = NoRegionHeight = 0;

	m_PixelSizeX = 0.;
	m_PixelSizeY = 0.;
	m_SavePixelSizeX = 0.;
	m_SavePixelSizeY = 0.;

	m_TopValue = 0;
	m_bShowInfo = TRUE;
	m_bContrast = FALSE;

	SaveWinWidth = 0.;
	SaveWinCenter = 0.;
	m_RotateState = 0;
	m_bUpDown = FALSE;
	m_bLeftRight = FALSE;

	SaveSltXpos = SaveSltYpos = SaveSltWidth = SaveSltHeight = 0;

	m_bShow_One_Localizer = FALSE;
	m_Highlight_ImageNum = 0;
	m_Highlight_SeriesNum = 0;
	m_NumOfLocalizers = 0;
	m_bLocalizer = FALSE;
	m_bShowLocalizers = FALSE;
	m_bLocalizerOK = FALSE;

	m_NumberOfNo = 0;
	m_CurrentNo = 0;
	m_bShowNo = TRUE;
	//	m_hdib=0;

	m_ImageNum_Array.RemoveAll();
	m_SeriesNum_Array.RemoveAll();
	NotationArray.RemoveAll();

}
CDCMImage::~CDCMImage()
{
	int         i;

	if (PixelValueData != NULL)
	{
		delete[]PixelValueData;
		PixelValueData = NULL;
	}

	if (WatchData != NULL)
	{
		delete[]WatchData;
		WatchData = NULL;
	}

	if (SavePixelValueData != NULL)
		delete[] SavePixelValueData;

	m_ImageNum_Array.RemoveAll();
	m_ImageNum_Array.FreeExtra();

	m_SeriesNum_Array.RemoveAll();
	m_SeriesNum_Array.FreeExtra();

	for (i = 0;i < m_NumberOfNo;i++)
	{
		delete NotationArray[i];
	}

	NotationArray.RemoveAll();
	NotationArray.FreeExtra();

	//	::GlobalFree((HGLOBAL)m_hdib);
}
int  CDCMImage::GetPixelData(CString FilePath)
/***return 1 if getbpixel data successfully ************/
/***return 0 if failed---------------------------------*/
{
	FILE* fp;
	char	 ch;
	char     ch2[3];
	int		 dicm_success = 0, transfer_syntax_success = 0, Pixel_success = 0;
	int      little_endian_specified2;
	int      buff_allocated = 0;
	int      group, element, oldGroup, oldElement;//group tag,element tag
	long     vlength;
	int      i;
	char     sTemp[500];

	if ((fp = fopen(FilePath, "rb")) == NULL)
		return   0;
	//==Add For Ours==//
	//fseek(fp,128,1);
	//==Add For Ours==//
	for (i = 0;i < 300;i++)
	{
		fread(&ch, sizeof(char), 1, fp);
		if (ch == 'D')
		{
			fread(&ch2, 3 * sizeof(char), 1, fp);
			if ((ch2[0] == 'I') && (ch2[1] == 'C') && (ch2[2] == 'M'))
			{
				dicm_success = 1;
				break;
			}
		}
	}

	little_endian_specified2 = 0;
	//===============================================================//
	//end of while,recognize 'DICM' successfully                     //
	//or read end of file                                            //
	//===============================================================//
	if (dicm_success)
	{
		little_endian_specified = 1;
		explicit_specified = 1;
		//===============================================================//
		//then get TransferSyntax in the 0002 group which is encoded in  //
		//little_endian transfer syntax,to do this in the following while//
		//loop                                                           //
		//===============================================================//

		oldElement = -1;
		oldGroup = 0x0002;

		while (1)
		{
			group = read16(fp);
			element = read16(fp);

			if (group != 0x0002)
			{
				oldGroup = group;
				oldElement = element;
				break;
			}

			if (!(group == 0x0002 && element > oldElement))
			{
				fclose(fp);
				return 0;//failed!//
			}

			oldElement = element;

			//===============================================================//
			//break out of the while statement,                              //
			//beacuse already read over the 0x0002 group                     //
			//===============================================================//
			fread(&ch2, 2 * sizeof(char), 1, fp);//read over VR
			if ((ch2[0] = 'O') && (ch2[1] == 'B'))
			{
				fseek(fp, 2L, 1);
				vlength = read32(fp);
			}
			else
				vlength = read16(fp);
			if ((group == 0x0002) && (element == 0x0010))
				// what we want to know is here!then read value:transfer syntax//
			{
				fread(sTemp, vlength * sizeof(char), 1, fp);
				sTemp[vlength] = 0;
				if (strcmp(sTemp, "1.2.840.10008.1.2") == 0)
				{
					little_endian_specified2 = 1;
					explicit_specified = 0;
				}
				else if (strcmp(sTemp, "1.2.840.10008.1.2.1") == 0)
					little_endian_specified2 = 1;
				else if (strcmp(sTemp, "1.2.840.10008.1.2.2") == 0)
					little_endian_specified2 = 2;
				else
					little_endian_specified2 = 3;
				transfer_syntax_success = 1;
			}
			else
				//seek  over value  //
			{
				fseek(fp, vlength, 1);
			}
		}//end of while
	}//end of if dicm_success
	else
		//'DICM' recognizing failed,suppose there is no head       //
		//          ,so move the pointer to the begin of the file !//
	{
		fseek(fp, 0L, 0);
		//==Add For Ours==//
		 //fseek(fp,128,1);
		//==Add For Ours==//
		little_endian_specified = 1;
		explicit_specified = 0;//implicit
		oldGroup = group = read16(fp);
		oldElement = element = read16(fp);
	}
	//===============================================================//
	if (little_endian_specified2 == 0)
		//little_endian_specified not given in the head ,so it is implicit //
		//and  little_endian_specified by default -------------------------//
	{
		little_endian_specified = 1;
		explicit_specified = 0;
	}
	else
		little_endian_specified = little_endian_specified2;

	if ((little_endian_specified != 1) && (little_endian_specified != 2))
	{
		fclose(fp);
		return    0;
	}
	else
	{
		//===============================================================//
		//the head is read over and transfer syntax is gotten            //
		//successfully,or is gotten by default ,                         //
		//             then to read the data set because the             //
		//encoding method is known .then to read each data               //
		// element once in  the following while loop.                    //                         
		//group,element is already having a value!                       //
		//===============================================================//
		if (!HandleDataSet(fp, group, element))
		{
			fclose(fp);
			return 0;
		}

		Pixel_success = m_pixel_success;

		//-----------------------------------------------------//
		//end  of while  ,Pixel data is encountered            //      
		//then to read pixel data in another while loop        //
		//-----------------------------------------------------//

	}
	//read Data Set over!//
	fclose(fp);

	return 1;

}
int CDCMImage::read16(FILE* fp)
{//this function reads a 16-bit entity/value,if  error return -1
	uint8 t1, t2;
	if (!fread(&t1, sizeof(char), 1, fp)) return -1;
	if (!fread(&t2, sizeof(char), 1, fp)) return -1;
	if (little_endian_specified == 1)
		return  t1 + t2 * 256;
	else //if(little_endian_specified==2)	
		return  t1 * 256 + t2;
}
//---------------------------------------------------------------------------//
int CDCMImage::read32(FILE* fp)
{//this function reads a 32-bit entity/value,if error return -1
	uint8 t1, t2, t3, t4;
	if (!fread(&t1, sizeof(char), 1, fp)) return -1;
	if (!fread(&t2, sizeof(char), 1, fp)) return -1;
	if (!fread(&t3, sizeof(char), 1, fp)) return -1;
	if (!fread(&t4, sizeof(char), 1, fp)) return -1;
	if (little_endian_specified == 1)
		return t1 + t2 * 256 + t3 * 256 * 256 + t4 * 256 * 256 * 256;
	else //if(little_endian_specified==2)	
		return  t1 * 256 * 256 * 256 + t2 * 256 * 256 + t3 * 256 + t4;
}
int  CDCMImage::HandleDataSet(FILE*& fp, int group, int element)
//given group and element handle dataset.//
{
	char     ch2[3];
	int      value_is_read;
	int      buff_allocated = 0;
	int      oldGroup, oldElement;//group tag,element tag
	long     vlength, reallength;
	int      ii, i, c;
	char     sTemp[500];
	char     sTemp1[500];
	vType    ValueType;

	slope_and_intercept_specified = 0;
	photometric_specified = 0;     //if not speicified ,least value is black     //
	number_of_frames_specified = 0;//if not specified ,there is one frame only   //
	frame_time_specified = 0;
	frame_time_vector_specified = 0;

	oldGroup = group;
	oldElement = element;

	while (1)
	{
		if (explicit_specified)
			//read over the VR          //
		{
			fread(&ch2, 2 * sizeof(char), 1, fp);
			if (((ch2[0] == 'S') && (ch2[1] == 'Q')) ||
				((ch2[0] == 'O') && (ch2[1] == 'B')) ||
				((ch2[0] == 'O') && (ch2[1] == 'W')) ||
				((ch2[0] == 'U') && (ch2[1] == 'N')))
				fseek(fp, 2L, 1);
		}
		if (!explicit_specified)
		{
			vlength = read32(fp);
		}
		else
		{
			if (((ch2[0] == 'S') && (ch2[1] == 'Q')) ||
				((ch2[0] == 'O') && (ch2[1] == 'B')) ||
				((ch2[0] == 'O') && (ch2[1] == 'W')) ||
				((ch2[0] == 'U') && (ch2[1] == 'N')))
				vlength = read32(fp);
			else
				vlength = read16(fp);
		}
		//=====================================================//
		//value length is known,but before we know how to read //
		// the value ,there is another thing needed to known,  //
		//that is the value type.                              //
		//all types are string,so should be read one byte by   //
		//one byte except accasions below,in these accasions   //
		//byte ordering should be considered:                  //
		//2 byte US,SS,OW                                      //
		//4 byte UL,SL,FL                                      //
		//8 byte FD                                            //
		//=====================================================//
		value_is_read = 0;
		//=====================================================//
		//not only save these values ,but also to mark that    //
		//we do have these values!-----------------------------//
		//=====================================================//
		ValueType = unknown;
		int     j;
		switch (group)
		{
		case 0x0008:
			switch (element)
			{
				//case 0x0000:
				//	vlength = read32(fp);
				//	break;
			case 0x0008:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				c = 0;
				j = 0;
				for (i = 0;i < vlength;i++)
				{
					sTemp1[j++] = sTemp[i];
					if ((sTemp[i] == '\\') || (i == vlength - 1))
					{
						if (sTemp[i] == '\\')     sTemp1[j - 1] = 0;
						else                   sTemp1[j] = 0;

						j = 0;
						if (strcmp(sTemp1, "LOCALIZER") == 0)
							m_bLocalizer = 1;
						else
							continue;
					}
				}
				break;
			case 0x0022:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(AcquisitionDate, sTemp);
				value_is_read = 1;
				break;
			case 0x0032:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(AcquisitionTime, sTemp);
				value_is_read = 1;
				break;
			case 0x0060:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(Modality, sTemp);
				value_is_read = 1;
				break;
			case 0x0080:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(InstitutionName, sTemp);
				value_is_read = 1;
				break;
			case 0x1090:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(ManufacturerMode, sTemp);
				value_is_read = 1;
				break;
			}
			break;
		case 0x0010:
			switch (element)
			{
			case 0x0050:
				//SQ//
				ValueType = SQ;
				break;
			case 0x0010:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(PatientName, sTemp);
				value_is_read = 1;
				break;
			case 0x0040:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(PatientSex, sTemp);
				value_is_read = 1;
				break;
			case 0x1010:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(PatientAge, sTemp);
				value_is_read = 1;
				break;
			case 0x0020:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(PatientID, sTemp);
				value_is_read = 1;
				break;
			}
			break;
		case 0x0018:
			switch (element)
			{
			case 0x0082:
			case 0x1032://SQ//
				ValueType = SQ;
				break;
			case 0x1063://DS :Frametime//
				break;
			case 0x1065://DS :FrameTime//
				//HOW TO GET THE ARRAY?//
				break;
			case 0x1066://DS :Frame Delay//
				break;
			case 0x0015:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(BodyPart, sTemp);
				value_is_read = 1;
				break;
			case 0x0050:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(SliceThickness, sTemp);
				value_is_read = 1;
				break;
			case 0x0060:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(KVP, sTemp);
				value_is_read = 1;
				break;
			case 0x1151:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(TubeCurrent, sTemp);
				value_is_read = 1;
				break;
			case 0x1084:
			case 0x1100:
			case 0x1110:
			case 0x1111:
			case 0x1115:
			case 0x1120:
			case 0x1125:
			case 0x1130:
			case 0x1140:
			case 0x1145:
			case 0x2112:
			case 0x6011://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x0020:
			switch (element)
			{
			case  0x0010:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(StudyID, sTemp);
				value_is_read = 1;
				break;
			case 0x0011:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(SeriesNumber, sTemp);
				value_is_read = 1;
				break;
			case 0x0013:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(ImageNumber, sTemp);
				value_is_read = 1;
				break;
			case 0x0032://Image Position//
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				c = 0;
				for (i = 0;i < vlength;i++)
				{
					if (sTemp[i] == '\\')
					{
						sTemp[i] = 0;
						ImagePosition[c++] = atof(sTemp);
						memset(sTemp, ' ', i + 1);
					}
					if (c == 2)
					{
						ImagePosition[2] = atof(sTemp);
						break;
					}
				}
				break;
			case 0x0037://Image Orientation//
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				c = 0;
				for (i = 0;i < vlength;i++)
				{
					if (sTemp[i] == '\\')
					{
						sTemp[i] = 0;
						ImageOrientation[c++] = atof(sTemp);
						memset(sTemp, ' ', i + 1);
					}
					if (c == 5)
					{
						ImageOrientation[5] = atof(sTemp);
						break;
					}
				}
				break;
			case 0x1041:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				strcpy(SliceLocation, sTemp);
				value_is_read = 1;
				break;
			}
			break;
		case 0x0028:
			switch (element)
			{
			case 0x0002://US//
				SamplesPerPixel = read16(fp);
				value_is_read = 1;
				//*if RGB display**//
				//if(SamplesPerPixel!=1)
				//	return  0;
				break;
			case 0x0004://CS//
				value_is_read = 1;
				photometric_specified = 1;
				fread(sTemp, vlength, 1, fp);
				for (ii = 0;;ii++)
					if (sTemp[ii] == 0x20)
					{
						sTemp[ii] = 0;
						break;
					}
				if (strcmp(sTemp, "MONOCHROME2") == 0)
					PhotometricInterpretation = 2;
				else if (strcmp(sTemp, "MONOCHROME1") == 0)
					PhotometricInterpretation = 1;
				else if (strcmp(sTemp, "RGB") == 0)
					PhotometricInterpretation = 4;
				else  PhotometricInterpretation = 3;
				break;
			case 0x0008://IS//
				fread(sTemp, vlength, 1, fp);
				sTemp[vlength] = 0;
				NumberOfFrames = atoi(sTemp);
				number_of_frames_specified = 1;
				value_is_read = 1;
				break;
			case 0x0009://AT//
				value_is_read = 1;
				fread(sTemp, sizeof(char) * vlength, 1, fp);
				sTemp[vlength] = 0;
				if (strcmp(sTemp, "(0018,1063)") == 0)
					frame_time_specified = 1;
				else if (strcmp(sTemp, "(0018,1065)") == 0)
					frame_time_vector_specified = 1;
				break;
			case 0x0010://US//
				Rows = read16(fp);
				m_SaveImageHeight = ImageHeight = Rows;
				value_is_read = 1;
				break;
			case 0x0011://US//
				Colums = read16(fp);
				m_SaveImageWidth = ImageWidth = Colums;
				value_is_read = 1;
				break;
			case 0x0030:
				fread(sTemp, sizeof(char), vlength, fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				for (i = 0; i < vlength;i++)
					if (sTemp[i] == '\\') break;
				m_PixelSizeY = (float)atof(sTemp);
				if (i >= vlength)
					m_PixelSizeX = m_PixelSizeY;
				else
				{
					sTemp[i] = 0;
					m_PixelSizeX = (float)atof(sTemp + i + 1);
				}
				m_SavePixelSizeX = m_PixelSizeX;
				m_SavePixelSizeY = m_PixelSizeY;
				break;
			case 0x0100://US//
				BitsAllocated = read16(fp);
				value_is_read = 1;
				break;
			case 0x0101://US//
				BitsStored = read16(fp);
				value_is_read = 1;
				break;
			case 0x0102://US//
				HighBit = read16(fp);
				value_is_read = 1;
				break;
			case 0x0103://US//
				PixelRepresentation = read16(fp);
				value_is_read = 1;
				break;
			case 0x0106:
				//Smallest Image Pixel Value//
				//US or SS,so,donot know how to handle//
				break;
			case 0x0107:
				//Largest Image Pixel Value//
				//US or SS                 //
				break;
			case 0x1050://DS//
				fread(sTemp, vlength, 1, fp);
				sTemp[vlength] = 0;
				WindowCenter = (float)atof(sTemp);
				value_is_read = 1;
				window_center_specified = 1;
				break;
			case 0x1051://DS//
				fread(sTemp, vlength, 1, fp);
				sTemp[vlength] = 0;
				WindowWidth = (float)atof(sTemp);
				value_is_read = 1;
				window_width_specified = 1;
				break;
			case 0x1052://SQ//
				fread(sTemp, vlength, sizeof(char), fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				slope_and_intercept_specified = 1;
				m_Intercept = atoi(sTemp);
				break;
			case 0x1053://SQ//
				fread(sTemp, vlength, sizeof(char), fp);
				sTemp[vlength] = 0;
				value_is_read = 1;
				slope_and_intercept_specified = 1;
				m_Slope = atoi(sTemp);
				break;
			case 0x3000://SQ//
				ValueType = SQ;
				break;
			case 0x3010://SQ//
				ValueType = SQ;
				break;
			}//end switch in the case//
			break;
		case 0x0032:
			switch (element)
			{
			case 0x1064://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x0038:
			switch (element)
			{
			case 0x0004:
			case 0x0044://SQ//
				ValueType = SQ;
				break;

			}
			break;
		case 0x0040://text
			break;
		case 0x2000:
			switch (element)
			{
			case 0x0500://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x2010:
			switch (element)
			{
			case 0x0500:
			case 0x0510:
			case 0x0520://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x2020:
			switch (element)
			{
			case 0x0110:
			case 0x0111:
			case 0x0130:
			case 0x0140://SQ/
				ValueType = SQ;
				break;
			}
			break;
		case 0x2040:
			switch (element)
			{
			case 0x0010:
			case 0x0500://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x2100:
			switch (element)
			{
			case 0x0500://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x4008:
			switch (element)
			{
			case 0x0050:
			case 0x0111:
			case 0x0117:
			case 0x0118://SQ//
				ValueType = SQ;
				break;
			}
			break;
		case 0x7fe0:
			switch (element)
			{
			case  0x0010://pixcel is encountered!
				if (!slope_and_intercept_specified)
				{
					m_Slope = 1;
					m_Intercept = 0;
				}

				if (!number_of_frames_specified)
					NumberOfFrames = 1;

				//  SamplesPerPixel=1; 
				   //*or 3*//
				reallength = Rows * Colums * SamplesPerPixel * NumberOfFrames * BitsAllocated / 8;
				fseek(fp, vlength - reallength, 1);
				//perhaps there is something unuseful before the pixel data,//
				//so, skip them.                                            //
				//see below ,mutiple frames is considered?                  //

				uint8* buff;
				int     n;
				int     bytes_per_pixel;
				int     unsigned_flag;

				bytes_per_pixel = BitsAllocated / 8 * SamplesPerPixel;
				unsigned_flag = 1 - PixelRepresentation;

				buff = (uint8*)malloc(ImageHeight * ImageWidth * bytes_per_pixel);

				n = fread(buff, ImageHeight * ImageWidth * bytes_per_pixel, 1, fp);

				if (little_endian_specified == 0)
					flip_16bit_data(buff, ImageWidth, ImageHeight);

				if (bytes_per_pixel == 1)
				{
					if (unsigned_flag)   PixelValueData = to_32bit_data((uint8*)buff, ImageWidth, ImageHeight);
					else                PixelValueData = to_32bit_data((int8*)buff, ImageWidth, ImageHeight);
				}
				else if (bytes_per_pixel == 2)
				{
					if (unsigned_flag)   PixelValueData = to_32bit_data((uint16*)buff, ImageWidth, ImageHeight);
					else                PixelValueData = to_32bit_data((int16*)buff, ImageWidth, ImageHeight);
				}

				if (buff != NULL)          delete[]buff;

				handle_photometric_interpretation(PixelValueData, ImageWidth, ImageHeight);

				if (slope_and_intercept_specified)
					apply_slope_intercept(PixelValueData, ImageWidth, ImageHeight, m_Slope, m_Intercept);

				MinPixelValue = MaxPixelValue = PixelValueData[0];
				for (i = 0;i < ImageWidth * ImageHeight;i++)
				{
					if (PixelValueData[i] > MaxPixelValue)
						MaxPixelValue = PixelValueData[i];
					if (PixelValueData[i] < MinPixelValue)
						MinPixelValue = PixelValueData[i];
				}

				if ((!window_center_specified) || (!window_width_specified))
				{
					WindowWidth = (float)(MaxPixelValue - MinPixelValue);
					WindowCenter = (float)(MaxPixelValue - MinPixelValue) / 2 + MinPixelValue;
				}

				//像素值在PixelValueData[] 里
				//每行有ImageWidth 个像素
				//有ImageHeight 行//////////////////////////////////////////////



				/*		CFile	file;
						PWORD	pPixel;
						DWORD   d;
						int xSize,ySize;
						xSize=ySize=512;
						CFileException e;
						file.Open("e:\\zgh\\hehe", CFile::modeCreate | CFile::modeWrite, &e );
						pPixel = new WORD[xSize*ySize];
						for(j=0;j<ySize;j++)
							for(i=0;i<xSize;i++)
							{
								d= PixelValueData[j*ImageWidth+i];
								*(pPixel+j*ySize+i)=(WORD)((d-MinPixelValue)*2300./(MaxPixelValue-MinPixelValue));
							}
						file.Write(pPixel, xSize*ySize*2);
						file.Close();
				*/
				////////////////////////////////////////////////////////////////

				SaveWinWidth = WindowWidth;
				SaveWinCenter = WindowCenter;
				m_SaveImageWidth = ImageWidth;
				m_SaveImageHeight = ImageHeight;

				if (SavePixelValueData == NULL)
					SavePixelValueData = new int32[ImageWidth * ImageHeight];
				if (SavePixelValueData == NULL)
				{
					AfxMessageBox("No Enough Memory!");
					return 0;
				}

				for (i = 0;i < ImageHeight * ImageWidth;i++)
					SavePixelValueData[i] = PixelValueData[i];

				//	CmpOrientationMark();

					//pixel is read ok.//
				return 100;//okokokokokokokokokokokoko
			   ///////////////////////////////////////
				break;
			}

		}

		if (!value_is_read)
			//if((ValueType==SQ)&&(vlength==0xffffffff))
			if (vlength == 0xffffffff)//an sequence  of items with undefined length is found,handle it.//
			{
				while (1)
				{
					group = read16(fp);
					element = read16(fp);

					if (group == 0xfffe && element == 0xe0dd)
					{
						//a data element is ok ,handle next.//
						fseek(fp, 4, 1);
						break;
					}

					HandleItem(fp, group, element);

				}//wnd of while.//

			}
			else
				fseek(fp, vlength, 1);

		group = read16(fp);
		element = read16(fp);
		//	
		if (!((group == oldGroup && element > oldElement) || (group > oldGroup)))
		{
			return 0;//read failed!//
		}
		//when encountered with 0xfffe group ,a dataset is handled over ,so return;
		if (group == 0xfffe && element == 0xe000)
			return 1;//item
		if (group == 0xfffe && element == 0xe00d)
			return 2;//item delim  item.
		if (group == 0xfffe && element == 0xe0dd)
			return 3;//sequence delim item.


		oldGroup = group;
		oldElement = element;

	}//end of while///////////////////////
	return 1;
}
void CDCMImage::flip_16bit_data(uint8* buff, int width, int height) {
	//
	//this function flips i.e. reverses the 'endian-ness' of the input data
	//(when the input is a 2d array).
	//
	if (BitsAllocated != 16)  return;

	for (int i = 0; i < 2 * width * height; i += 2) {
		uint8 tmp = buff[i];
		buff[i] = buff[i + 1];
		buff[i + 1] = tmp;
	}

}
//---------------------------------------------------------------------------//
int32* CDCMImage::to_32bit_data(uint8* buff, int width, int height) {

	int32* tmp = (int32*)malloc((sizeof * tmp) * width * height);
	memset(tmp, 0, (sizeof * tmp) * width * height);
	for (int i = 0; i < (width * height); i++)  tmp[i] = buff[i];
	return tmp;
}
//---------------------------------------------------------------------------//
int32* CDCMImage::to_32bit_data(int8* buff, int width, int height) {

	int32* tmp = (int32*)malloc((sizeof * tmp) * width * height);
	memset(tmp, 0, (sizeof * tmp) * width * height);
	for (int i = 0; i < (width * height); i++)  tmp[i] = buff[i];
	return tmp;
}
//---------------------------------------------------------------------------//
int32* CDCMImage::to_32bit_data(uint16* buff, int width, int height) {

	int32* tmp = (int32*)malloc((sizeof * tmp) * width * height);
	memset(tmp, 0, (sizeof * tmp) * width * height);
	for (int i = 0; i < (width * height); i++)  tmp[i] = buff[i];
	return tmp;
}
//---------------------------------------------------------------------------//
int32* CDCMImage::to_32bit_data(int16* buff, int width, int height) {

	int32* tmp = (int32*)malloc((sizeof * tmp) * width * height);
	memset(tmp, 0, (sizeof * tmp) * width * height);
	for (int i = 0; i < (width * height); i++)  tmp[i] = buff[i];
	return tmp;
}
void CDCMImage::handle_photometric_interpretation(int32* tmp,
	int width, int height) {
	if (tmp == NULL)  return;
	if (width <= 0)     return;
	if (height <= 0)     return;
	if (!photometric_specified || PhotometricInterpretation == 2)
		return;
	long maxValue = 1 << BitsStored;
	//assert(maxValue == (1 << BitsStored));
	for (long i = 0; i < width * height; i++)  tmp[i] = maxValue - tmp[i];
}
void CDCMImage::apply_slope_intercept(int32* buff, int width, int height,
	double slope, double intercept) {
	//
	//apply the slope & intercept to change contrast.
	//
	if (m_Slope == 1.0 && m_Intercept == 0.0)  return;

	double m = slope;
	double b = intercept;

	int i;
	int size = width * height;
	for (i = 0; i < size; i++) {
		buff[i] = (int32)(buff[i] * m + b);
	}

}
int   CDCMImage::HandleItem(FILE*& fp, int group, int element)
//fp is juxt moved over tag.//
{
	int  length;

	length = read32(fp);

	if (length == 0xffffffff)
	{
		group = read16(fp);
		element = read16(fp);

		HandleDataSet(fp, group, element);
		fseek(fp, 4, 1);
		//the length of this item is undefined,so  there is a //
		//itel delim item after the dataset. because the tag is already//
		//read in HandleDataSet(),so just move over 4 BYTES.//
	}
	else
	{
		fseek(fp, length, 1);
		//length of the item is given ,so just move over.//
	}

	return 1;
}