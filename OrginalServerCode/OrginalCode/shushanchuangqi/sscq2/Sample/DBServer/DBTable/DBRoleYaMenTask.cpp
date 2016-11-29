#include "DBRoleYaMenTask.h"
void CDBRoleYaMenTaskTable::InitDataByStr(string & strData)
{
    CInArchive ar(strData);
    ar >> static_cast<SYaMenTaskInfo&>(*this);
}
bool CDBRoleYaMenTaskTable::UpdateDataByStr(string& strData)
{
    CInArchive ar(strData);
    SYaMenTaskInfo newData;
    ar >> newData;
    DIFF(dwHoly);
    DIFF(byHolyLevel);
    DIFF(byBuyTimes);
    DIFF(dwFlushTask1);
    DIFF(eState1);
    DIFF(dwFlushTask2);
    DIFF(eState2);
    DIFF(dwFlushTask3);
    DIFF(eState3);
    DIFF(dwFlushTask4);
    DIFF(eState4);
    DIFF(dwFlushTask5);
    DIFF(eState5);
    return  !_setChanged.none();
}

bool CDBRoleYaMenTaskTable::GetInsertSqlStr(string& sqlstr)
{

    mysqlpp::SQLStream ostr(NULL);
    ostr << "Insert into tblroleyamentaskinfo(qwRoleID,dwHoly,byHolyLevel,byBuyTimes,dwFlushTask1,eState1,dwFlushTask2,eState2,dwFlushTask3,eState3,dwFlushTask4,eState4,dwFlushTask5,eState5) Values(";
    ostr << (mysqlpp::sql_bigint_unsigned)_qwRoleID;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)dwHoly;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)byHolyLevel;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)byBuyTimes; 
    ostr << "," << (mysqlpp::sql_int_unsigned)dwFlushTask1;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)eState1;
    ostr << "," << (mysqlpp::sql_int_unsigned)dwFlushTask2;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)eState2;
    ostr << "," << (mysqlpp::sql_int_unsigned)dwFlushTask3;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)eState3;
    ostr << "," << (mysqlpp::sql_int_unsigned)dwFlushTask4;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)eState4;
    ostr << "," << (mysqlpp::sql_int_unsigned)dwFlushTask5;
    ostr << "," << (mysqlpp::sql_tinyint_unsigned)eState5;
    ostr << ");"; 
    sqlstr = ostr.str(); 
    return true;           
}

bool CDBRoleYaMenTaskTable::GetUpdateSqlStr(string& strsql)
{
    if(_setChanged.none())
        return false;
    mysqlpp::SQLStream ostr(NULL);
    ostr << "UPDATE tblroleyamentaskinfo SET";
    UPDATE_UINT_SAME(dwHoly);
    UPDATE_UTINYINT_SAME(byHolyLevel);
    UPDATE_UTINYINT_SAME(byBuyTimes);
    UPDATE_UINT_SAME(dwFlushTask1);
    UPDATE_UTINYINT_SAME(eState1);
    UPDATE_UINT_SAME(dwFlushTask2);
    UPDATE_UTINYINT_SAME(eState2);
    UPDATE_UINT_SAME(dwFlushTask3);
    UPDATE_UTINYINT_SAME(eState3);
    UPDATE_UINT_SAME(dwFlushTask4);
    UPDATE_UTINYINT_SAME(eState4);
    UPDATE_UINT_SAME(dwFlushTask5);
    UPDATE_UTINYINT_SAME(eState5);
    ostr << " WHERE   qwRoleID =" <<_qwRoleID <<  ";";  
    strsql = ostr.str();
    size_t pos = strsql.find("SET,");
    if(pos == string::npos)
        return false;
    strsql[pos+3] = ' ';
    return true;
}

void CDBRoleYaMenTaskTable::ResetChange(void)
{
    _setChanged.reset();
}

void CDBRoleYaMenTaskTable::InitDataByDBRow(const mysqlpp::Row &rRow)
{   
    GET_UINT_SAME(dwHoly);
    GET_UTINYINT_SAME(byHolyLevel);
    GET_UTINYINT_SAME(byBuyTimes); 
    GET_UINT_SAME(dwFlushTask1);
    GET_UINT_SAME(dwFlushTask2);
    GET_UINT_SAME(dwFlushTask3);
    GET_UINT_SAME(dwFlushTask4);
    GET_UINT_SAME(dwFlushTask5);
    UINT8 byState1;
    GET_UTINYINT(eState1,byState1);
    eState1 = static_cast<NYaMenTaskProt::EYaMenTaskState>(byState1);
    UINT8 byState2;
    GET_UTINYINT(eState2,byState2);
    eState2 = static_cast<NYaMenTaskProt::EYaMenTaskState>(byState2);
    UINT8 byState3;
    GET_UTINYINT(eState3,byState3);
    eState3 = static_cast<NYaMenTaskProt::EYaMenTaskState>(byState3); 
    UINT8 byState4;
    GET_UTINYINT(eState4,byState4);
    eState4 = static_cast<NYaMenTaskProt::EYaMenTaskState>(byState4); 
    UINT8 byState5;
    GET_UTINYINT(eState5,byState5);
    eState5 = static_cast<NYaMenTaskProt::EYaMenTaskState>(byState5); 
    TableRowBase::InitDataByDBRow(rRow);
}

void CDBRoleYaMenTaskTableManager::QueryDataByRoleId(UINT64 qwRoleID,QueryFun queryfun)
{
    mysqlpp::SQLStream basesql(NULL);
    basesql << "SELECT * FROM tblroleyamentaskinfo  WHERE qwRoleID = " << qwRoleID << ";";
    UINT64 qwLoadTimer = GetTickCount();
    CDBMgr::Instance().DoRoleSelectQuery(basesql.str(),_byLine,[this, qwRoleID, queryfun, basesql,qwLoadTimer] (INT8 byRet, mysqlpp::StoreQueryResult& dataRes) mutable {
            LOG_SQL << "[" << Time().hhmmssuu() << "]" << "do sql = " << basesql.str() << "result:" << byRet;
            LOG_INF<<"--------Load tblroleyamentaskinfo is "<<GetTickCount() - qwLoadTimer<<" miscseconds";
            UINT32 ncount = dataRes.size();
            if(byRet == 0 && ncount == 1)
            {
            mysqlpp::Row& rRow = dataRes[0];
            _RoleYaMenTaskData.InitDataByDBRow(rRow);
            }
            queryfun(byRet, ncount);
            });
}

void CDBRoleYaMenTaskTableManager::Update(string& strData, bool bIsAdd)
{
    CInArchive testAr(strData);
    SYaMenTaskInfo testData;
    testAr >> testData;
    _RoleYaMenTaskData.Update(strData, bIsAdd);
}

void CDBRoleYaMenTaskTableManager::GetALLInfo(RoleDataInfo & rData)
{
    rData.sYaMenInfo = (static_cast<SYaMenTaskInfo>(_RoleYaMenTaskData));
}

void CDBRoleYaMenTaskTableManager::DumpToDB(ManagerUpdateFun fun)
{
    _nNowQuery = GetUpdataNum();
    _nMaxQuery = _nNowQuery;
    _nErrQuery = 0;
    if (_RoleYaMenTaskData.HasOP())
    {
        _RoleYaMenTaskData.DumpToDB(_byLine,[fun,this](INT8 byRet)               {
                this->_nNowQuery--;
                if (byRet)
                {
                this->_nErrQuery++;
                }
                if (this->_nNowQuery == 0)
                {
                fun(_nMaxQuery,this->_nErrQuery);
                }
                });
    }


}

UINT32 CDBRoleYaMenTaskTableManager::GetUpdataNum()
{
    return  _RoleYaMenTaskData.HasOP() ? 1 : 0;

}

void CDBRoleYaMenTaskTableManager::DelData(string& strData)
{
}

