function InArea1()
    local user = GameScript:GetUser()
    LOG_WRN('Do ' .. tostring(user:GetUserID()) .. 'OnArea1' )
	local pPlayer = SharedToPtr(user:GetPlayer())
    if (nil == pPlayer) then
        LOG_WRN('NO Player')
        return 
    end
    local pDgn = SharedToPtr(pPlayer:GetDgn())
    if (pDgn == nil) then
        LOG_WRN('NO Dgn')
        return
    end
	pDgn:DestroyBlockByIndex(1)
end


--local user = GameScript:GetUser() ���user
--local pPlayer = SharedToPtr(user:GetPlayer()) ���pPlayer
--local pDgn = SharedToPtr(pPlayer:GetDgn()) ���pDgn
--pPlayer:FitherToMonster(wMonsterGroup)//ս��
--pDgn:CreateMonster(wMonsterGroup,fX,fY)//��������
--pDgn:DestroyMonster(wMonsterGroup)//�������
--pDgn:CreateGather(wGatherId,fX,fY)//�����ɼ���
--pDgn:DestroyGather(MonsterGroup)//���
--pDgn:TransPlayer(Player,wMap,fX,fy) ://���ͣ�������ĵ�ͼ
--pDgn:ClearMist(wMist)//�������
--pDgn:AddMist(wMist)//�������
--User:NoticeStory(wStroyID)//��������
--pPlayer:ElasticFrame(wType)//����
--pDgn:SetTimer(wTimerId,dwTimerIntral)//���ö�ʱ������ʱ��id��ʱ��
--pDgn:EndTimer(wTimerId)//���̽�����ʱ��
--pDgn:SetParam(wParamId,dwParamVal)//�洢��ʱ����
--pDgn:GetParam(wParamId)//��ȡ��ʱ����
--pDgn:CreateBlockByIndex(wIndex)//����·��
--pDgn:DestroyBlockByIndex(wIndex)//ɾ��·��
