#include "include/StdAfx.h"
#include "include/cfg.h"


extern PARAMETER_CONFIG	gCfg;

int
DataBaseInsert(
	char		*Name,				
//	char		*Building,
//	char		*Room,
	char		*Place,
	char		*Description,
	char		*Sn
	)
{
	_ConnectionPtr	m_pConnection;

	CoInitialize(NULL);

	// connect to SQL server
	try
	{
		m_pConnection.CreateInstance("ADODB.Connection");
		m_pConnection->ConnectionTimeout = 20;
		m_pConnection->Open(gCfg.database_connect, //DB_SERVER,
							"", 
							"", 
							adModeUnknown);
		if (m_pConnection->State == 0)
			return -1;
	}
	catch(_com_error &e)
	{
		m_pConnection = NULL;
//		AfxMessageBox(e.Description());		///error information
		return -1;
	}

	_CommandPtr		m_pCommand;						//
	m_pCommand.CreateInstance("ADODB.Command");		//init instance
	m_pCommand->ActiveConnection = m_pConnection;	//connect to the database

	// get room ID 
	_RecordsetPtr	m_pRecordset;
	_variant_t		var;
	char			*roomID = NULL;
	char			*buildingID = NULL;
	char			str_cmd[256];
//	char			req_num[50];

/*
	str_cmd[0] = '\0';
	sprintf(str_cmd, gCfg.database_search, Building, Room);
//	AfxMessageBox(str_cmd);

	try
	{
		m_pCommand->CommandText = str_cmd;		///command string
		m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdText);///ִ�����ȡ�ü�¼�� 
		var = m_pRecordset->GetCollect("RoomID");
		if(var.vt != VT_NULL)									//�жϼ�¼�ڸ���û����
			roomID = _com_util::ConvertBSTRToString((_bstr_t)var);

		m_pRecordset->Close();
		m_pRecordset = NULL;  
	}
	catch(_com_error &e)
	{
//		m_pRecordset = NULL;

//		m_pConnection->Close();
//		m_pConnection = NULL;

//		AfxMessageBox(e.Description());///��ʾ������Ϣ 
//		return -2;

		//using default ID of Information-Network-Center
		roomID = "781";
	}
//*/
	char			item[32];
	char			*pch;
	char			*pTemp = Place;
	char			isFoundBuilding = 0;

	pch = item;
	while(1)
	{
		if (*pTemp == ' ' || *pTemp == '\0')
		{
			*pch = '\0';
			pch = item;
		}
		else
		{
			*pch++ = *pTemp++;
			continue;
		}
/*		
		pTemp++;
		pch++;

		if (*pTemp != ' ' && *pTemp != '\0')
		{
			*pch++ = *pTemp++;
			continue;
		}

		*pch = '\0';
		pch = item;
//*/
		str_cmd[0] = '\0';
		sprintf(str_cmd, gCfg.database_search_department, item);

		try
		{
			m_pCommand->CommandText = str_cmd;		///command string
			m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdText);///ִ�����ȡ�ü�¼�� 
			var = m_pRecordset->GetCollect("roomid");
			m_pRecordset->Close();
			m_pRecordset = NULL; 

			if(var.vt != VT_NULL)									//�жϼ�¼�ڸ���û����
			{
				roomID = _com_util::ConvertBSTRToString((_bstr_t)var);	
				break;
			}
		}
		catch(_com_error &e)
		{
			m_pRecordset->Close();
			m_pRecordset = NULL;
		}
	
		if (!isFoundBuilding)
		{
			str_cmd[0] = '\0';
			sprintf(str_cmd, gCfg.database_search_building, item);

			try
			{
				m_pCommand->CommandText = str_cmd;		///command string
				m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdText);///ִ�����ȡ�ü�¼�� 
				var = m_pRecordset->GetCollect("buildingid");
				m_pRecordset->Close();
				m_pRecordset = NULL; 

				if(var.vt != VT_NULL)									//�жϼ�¼�ڸ���û����
				{
					buildingID = _com_util::ConvertBSTRToString((_bstr_t)var);	
					isFoundBuilding = 1;
				}
			}
			catch(_com_error &e)
			{
				m_pRecordset->Close();
				m_pRecordset = NULL;
			}
		}//end if (!isFoundBuilding)
		else
		{
			str_cmd[0] = '\0';
			sprintf(str_cmd, gCfg.database_search_room, buildingID, item);

			try
			{
				m_pCommand->CommandText = str_cmd;		///command string
				m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdText);///ִ�����ȡ�ü�¼�� 
				var = m_pRecordset->GetCollect("RoomID");
				m_pRecordset->Close();
				m_pRecordset = NULL; 

				if(var.vt != VT_NULL)									//�жϼ�¼�ڸ���û����
				{
					roomID = _com_util::ConvertBSTRToString((_bstr_t)var);	
					break;
				}
			}
			catch(_com_error &e)
			{
				m_pRecordset->Close();
				m_pRecordset = NULL;
			}
		}//end else

		if (*pTemp == '\0')
			break;
		pTemp++;
	}//end while
	

	if (roomID == NULL)
		//using default ID of Information-Network-Center
		roomID = "781";
		


	// storage process
	try
	{
		_ParameterPtr   pParamName;  
		pParamName.CreateInstance("ADODB.Parameter");  
		pParamName->Name="poser";			//���ô洢���̲�������  
		pParamName->Type=adChar;			//��������  
		pParamName->Size=200;				//������С  
		pParamName->Direction=adParamInput;	//�������������  
		pParamName->Value=_variant_t(Name);  
		m_pCommand->Parameters->Append(pParamName); 

		_ParameterPtr   pParamTitle;  
		pParamTitle.CreateInstance("ADODB.Parameter");  
		pParamTitle->Name="title";			//���ô洢���̲�������  
		pParamTitle->Type=adChar;			//��������  
		pParamTitle->Size=200;				//������С  
		pParamTitle->Direction=adParamInput;	//�������������  
		pParamTitle->Value=_variant_t("�������ϼ�¼");  
		m_pCommand->Parameters->Append(pParamTitle); 

		_ParameterPtr   pParamDescription;  
		pParamDescription.CreateInstance("ADODB.Parameter");  
		pParamDescription->Name="description";			//���ô洢���̲�������  
		pParamDescription->Type=adChar;							//��������  
		pParamDescription->Size=1000;							//������С  
		pParamDescription->Direction=adParamInput;				//�������������  
		pParamDescription->Value=_variant_t(Description);  
		m_pCommand->Parameters->Append(pParamDescription);

		_ParameterPtr   pParamReqSn;  
		pParamReqSn.CreateInstance("ADODB.Parameter");  
		pParamReqSn->Name="reqSn";			//���ô洢���̲�������  
		pParamReqSn->Type=adChar;							//��������  
		pParamReqSn->Size=50;							//������С  
		pParamReqSn->Direction=adParamInput;				//�������������  
		pParamReqSn->Value=_variant_t(Sn);  
		m_pCommand->Parameters->Append(pParamReqSn);

		_ParameterPtr   pParamRoomId;  
		pParamRoomId.CreateInstance("ADODB.Parameter");  
		pParamRoomId->Name="roomId";			//���ô洢���̲�������  
		pParamRoomId->Type=adInteger;							//��������  
		pParamRoomId->Size=4;							//������С  
		pParamRoomId->Direction=adParamInput;				//�������������  
		pParamRoomId->Value=_variant_t("781");  
		m_pCommand->Parameters->Append(pParamRoomId);

		_ParameterPtr   pParamError;  
		pParamError.CreateInstance("ADODB.Parameter");  
		pParamError->Name="error";			//���ô洢���̲�������  
		pParamError->Type=adInteger;							//��������  
		pParamError->Size=4;							//������С  
		pParamError->Direction=adParamOutput;				//�������������  
		m_pCommand->Parameters->Append(pParamError);
		
		m_pCommand->CommandText = "omm_sys_CreateWF";	//storage process name
		m_pCommand->CommandType=adCmdStoredProc;//��ʾΪ�洢����adCmdStoredProc  
		m_pCommand->Execute(NULL, NULL, adCmdStoredProc); 

		m_pCommand = NULL;
	}
	catch(_com_error &e)
	{
		m_pCommand = NULL;

		m_pConnection->Close();
		m_pConnection = NULL;

//		AfxMessageBox(e.Description());///��ʾ������Ϣ
		return -3;
	}

	m_pConnection->Close();
    m_pConnection = NULL;

	return 0;
}

#ifdef PPP

int
DataBaseTest(void)
{
	int		ret;

	ret = DataBaseInsert("����", "��ѧ¥", "1003", "��������", "200909071601");

	return ret;
}

#endif //PPP