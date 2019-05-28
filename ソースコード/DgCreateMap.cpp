#include"DgCreateMap.h"
#include<math.h>
#include<time.h>
#include<algorithm>
#include<functional>
#include<iostream>

//=====================================================
//  ���̉�
//=====================================================
int DgCreateMap::m_Map[MAP_HEIGHT][MAP_WIDTH] = { 0 };


//=====================================================================================
// �֐����FCreate
// ���e�@�F�t�B�[���h��������
// �߂�l�FDgCreateMap*
//=====================================================================================
DgCreateMap* DgCreateMap::Create()
{
	// �V�[�h����
	srand((unsigned)time(NULL));
	rand();

	// �c�������炩���������炩�����߂�
	int luck = rand() % 2;
	luck = 1;

	DgCreateMap* map = new DgCreateMap();

	//==============================================
	// ��������
	//==============================================
	if (luck == 0)
	{
		float X = static_cast<float>((rand() % (MAP_WIDTH / 2)) + 5);		// 5 �` ( 5 + WIDTH / 2)�̊ԁi�c�����j

		// �c����
		DgCreateMap::RegistArea(map, D3DXVECTOR2(0.0f, 0.0f),     D3DXVECTOR2(X - 1.0f, static_cast<float>(MAP_HEIGHT) - 1.0f));
		DgCreateMap::RegistArea(map, D3DXVECTOR2(X + 1.0f, 0.0f), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1.0f, static_cast<float>(MAP_HEIGHT) - 1.0f));
		DgCreateMap::RegistRoot(map, D3DXVECTOR2(X, 0.0f),        D3DXVECTOR2(X, static_cast<float>(MAP_HEIGHT) - 1.0f));

		DgCreateMap::WriteRootToMap(D3DXVECTOR2(X, 0.0f), D3DXVECTOR2(X, MAP_HEIGHT - 1.0f));
	}
	else if (luck == 1)
	{
		float Y = static_cast<float>((rand() % (MAP_HEIGHT / 2)) + 5);		// 5 �` ( 5 + HEIGHT / 2)-30�̊ԁi�������j
														// ������
		DgCreateMap::RegistArea(map, D3DXVECTOR2(static_cast<float>(0), static_cast<float>(0)), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y - 1));
		DgCreateMap::RegistArea(map, D3DXVECTOR2(static_cast<float>(0), Y + 1),					D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, static_cast<float>(MAP_HEIGHT) - 1));
		DgCreateMap::RegistRoot(map, D3DXVECTOR2(static_cast<float>(0), Y),						D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y));

		DgCreateMap::WriteRootToMap(D3DXVECTOR2(static_cast<float>(0), Y), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y));

	}

	//==============================================
	// �G���A�̕���
	//==============================================
	for (int i = 1; i < (AREA_NUM - 1); ++i)
	{
		luck++;
		luck = luck % 2;

		// �ő�}�b�v�T�C�Y�̌���
		AREA_LIST maxArea = std::for_each(map->areaList.begin(), map->areaList.end(), MaxArea()).GetMaxArea();

		if (maxArea.MaximizeArea->GetWidth() > maxArea.MaximizeArea->GetHeight())
		{
			// �c����
			int X = maxArea.MaximizeArea->GetWidth() / 2;
			bool guusuu = (maxArea.MaximizeArea->GetWidth() % 2) == 0;

			// ����
			if (guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X - 1), maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->end.y));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->end.y));

			}
			// �
			else if (!guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 2), maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->end.y));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->end.y));
			}
		}
		else
		{
			// ������
			int Y = maxArea.MaximizeArea->GetHeight() / 2;
			bool guusuu = (maxArea.MaximizeArea->GetWidth() % 2) == 0;

			// ����
			if (guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y - 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - Y));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - Y));

			}
			// �
			else if (!guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 2)));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y + 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y + 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));

			}
		}

		// �I�΂ꂽ�G���A�̍폜
		int itrnum = 0;
		for (auto itr = map->areaList.begin(); itr != map->areaList.end(); ++itr)
		{
			if (itrnum == maxArea.num)
			{
				delete (*itr);
				map->areaList.erase(itr);
				break;
			}
			itrnum++;
		}
	}

	

	//==============================================
	// ���������
	//==============================================
	int itrnum = 0;
	for (int i = 0; i < AREA_NUM; ++i)
	{
		D3DXVECTOR2 start, end;
		for (auto itr = map->areaList.begin(); itr != map->areaList.end(); ++itr)
		{
			if (itrnum == i)
			{
				start = (*itr)->start;
				end = (*itr)->end;
				break;
			}
			itrnum++;
		}
		itrnum = 0;

		// �ǂ̌�����ύX
		float random = static_cast<float>(rand() % 2 + 1);
		//float random2 = rand() % 2 + 1;
		D3DXVECTOR2 wallWidth = { random, random };

		DgCreateMap::RegistRoom(map, start + wallWidth, end - wallWidth);
		DgCreateMap::WriteRoomToMap(start + wallWidth, end - wallWidth);
	}

	//==============================================
	// ��������root�����邩�`�F�b�N
	//==============================================
	for (auto itr = map->roomList.begin(); itr != map->roomList.end(); ++itr)
	{
		int x;
		int y;

		// ���[����������W�����߂�
		(*itr)->ConnectPoint.x = (*itr)->start.x + rand() % (*itr)->GetWidth();
		(*itr)->ConnectPoint.y = (*itr)->start.y + rand() % (*itr)->GetHeight();

		// ���root�����邩�ǂ���
		x = static_cast<int>((*itr)->ConnectPoint.x);
		y = static_cast<int>((*itr)->start.y - 1);

		while (1)
		{
			// ���݂���
			if (m_Map[y][x] == 1)
			{
				(*itr)->CrossLoad[0] = D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y));
				(*itr)->GateExist[0] = true;
				break;
			}
			y--;
			if (y == -1)
			{
				(*itr)->GateExist[0] = false;
				break;
			}
		}

		// ����root�����邩�ǂ���
		x = static_cast<int>((*itr)->ConnectPoint.x);
		y = static_cast<int>((*itr)->end.y + 1);

		while (1)
		{

			// ���݂���
			if (m_Map[y][x] == 1)
			{
				(*itr)->CrossLoad[1] = D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y));
				(*itr)->GateExist[1] = true;
				break;
			}
			y++;
			if (y == MAP_HEIGHT)
			{
				(*itr)->GateExist[1] = false;
				break;
			}
		}

		// ����root�����邩�ǂ���
		x = static_cast<int>((*itr)->start.x - 1);
		y = static_cast<int>((*itr)->ConnectPoint.y);

		while (1)
		{

			// ���݂���
			if (m_Map[y][x] == 1)
			{
				(*itr)->CrossLoad[2] = D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y));
				(*itr)->GateExist[2] = true;
				break;
			}
			x--;
			if (x == -1)
			{
				(*itr)->GateExist[2] = false;
				break;
			}
		}

		// �E��root�����邩�ǂ���
		x = static_cast<int>((*itr)->end.x + 1);
		y = static_cast<int>((*itr)->ConnectPoint.y);

		while (1)
		{

			// ���݂���
			if (m_Map[y][x] == 1)
			{
				(*itr)->CrossLoad[3] = D3DXVECTOR2(static_cast<float>(x), static_cast<float>(y));
				(*itr)->GateExist[3] = true;
				break;
			}
			x++;
			if (x == MAP_WIDTH)
			{
				(*itr)->GateExist[3] = false;
				break;
			}
		}

		// �����������Q�[�g�̍ő吔�̎Z�o
		for (int i = 0; i < 4; ++i)
		{
			if ((*itr)->GateExist[i])
			{
				(*itr)->MaxGate++;
			}

		}

		// �����̃Q�[�g����root�܂ł��q����
		(*itr)->GateNum = 1 + rand() % (*itr)->MaxGate;			// �Q�[�g���������_���Ō��߂�

		while (1)
		{
			for (int i = 0; i < 4; ++i)
			{
				if ((*itr)->GateExist[i])
				{
					WriteLoadToMap((*itr)->ConnectPoint, (*itr)->CrossLoad[i], i);
					(*itr)->GateNum--;
				}

				if ((*itr)->GateNum == 0)
				{
					break;
				}
			}

			if ((*itr)->GateNum == 0)
			{
				break;
			}

		}

	}

	//==============================================
	// �]�v�ȒʘH���폜����
	//==============================================
	for (auto itr = map->rootList.begin(); itr != map->rootList.end(); ++itr)
	{
		DgCreateMap::DeleteLine((*itr)->start, (*itr)->end);
	}


	//=============================================
	// ���������
	//=============================================
	//std::for_each(map->areaList.begin(), map->areaList.end(), [&]() {});
	for (auto itr = map->areaList.begin(); itr != map->areaList.end(); ++itr)
	{
		delete (*itr);
	}
	for (auto itr = map->rootList.begin(); itr != map->rootList.end(); ++itr)
	{
		delete (*itr);
	}
	for (auto itr = map->roomList.begin(); itr != map->roomList.end(); ++itr)
	{
		delete (*itr);	
	}
	map->areaList.clear();
	map->rootList.clear();
	map->roomList.clear();

	return map;
}

//=====================================================================================
// �֐����FRelease
// ���e�@�F�t�B�[���h���
//=====================================================================================
void DgCreateMap::Release()
{

	for (int i = 0; i < MAP_HEIGHT; ++i)
	{
		for (int j = 0; j < MAP_WIDTH; ++j)
		{

			m_Map[i][j] = 0;
		}
	}

}

//=====================================================================================
// �֐����FRegistArea
// ���e�@�F�_���W���������̗̈�o�^
//=====================================================================================
void DgCreateMap::RegistArea(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	Area* area = nullptr;
	area = new Area;
	area->start = start;
	area->end = end;
	map->areaList.push_back(area);
}

//=====================================================================================
// �֐����FRegistRoot
// ���e�@�F�_���W�������̃��[�g�o�^
//=====================================================================================
void DgCreateMap::RegistRoot(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	Root *root = nullptr;
	root = new Root();
	root->start = start;
	root->end = end;
	map->rootList.push_back(root);
}

//=====================================================================================
// �֐����FRegistRoom
// ���e�@�F�_���W�����G���A���̕����o�^
//=====================================================================================
void DgCreateMap::RegistRoom(DgCreateMap* map, D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	Room* room = nullptr;
	room = new Room;
	room->start = start;
	room->end = end;
	map->roomList.push_back(room);
}

//=====================================================================================
// �֐����FWriteRootToMap
// ���e�@�F�}�b�v�ɓ��̃��[�g����������
//=====================================================================================
void DgCreateMap::WriteRootToMap(D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	// �c������
	if (start.x == end.x)
	{
		for (int tate = static_cast<int>(start.y); tate <= static_cast<float>(end.y); tate++)
		{
			m_Map[tate][(int)start.x] = 1;
		}
	}
	// ��������
	else if (start.y == end.y)
	{
		for (int yoko = (int)start.x; yoko <= (int)end.x; yoko++)
		{
			m_Map[(int)start.y][yoko] = 1;
		}
	}
}

//=====================================================================================
// �֐����FWriteRoomToMap
// ���e�@�F�}�b�v�ɕ�������������
//=====================================================================================
void DgCreateMap::WriteRoomToMap(D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	for (int i = (int)start.y; i <= (int)end.y; ++i)
	{
		for (int j = (int)start.x; j <= (int)end.x; ++j)
		{
			m_Map[i][j] = 2;
		}
	}
}

//=====================================================================================
// �֐����FWriteLoadToMap
// ���e�@�F�����ɓ�����������
//=====================================================================================
void DgCreateMap::WriteLoadToMap(D3DXVECTOR2 start, D3DXVECTOR2 end, int mode)
{
	switch (mode)
	{
	case 0:		// ��
		while (1)
		{
			end.y++;
			if (m_Map[(int)end.y][(int)start.x] == 2) { break; }
			m_Map[(int)end.y][(int)start.x] = 1;

		}

		break;
	case 1:		// ��
		while (1)
		{
			end.y--;
			if (m_Map[(int)end.y][(int)start.x] == 2) { break; }
			m_Map[(int)end.y][(int)start.x] = 1;
		}

		break;
	case 2:		// ��
		while (1)
		{
			end.x++;
			if (m_Map[(int)start.y][(int)end.x] == 2) { break; }
			m_Map[(int)start.y][(int)end.x] = 1;
		}

		break;
	case 3:		// �E
		while (1)
		{
			end.x--;
			if (m_Map[(int)start.y][(int)end.x] == 2) { break; }
			m_Map[(int)start.y][(int)end.x] = 1;
		}

		break;
	default:
		break;
	}
}

//=====================================================================================
// �֐����FDeleteLine
// ���e�@�F�]�v�ȓ����폜
//=====================================================================================
void DgCreateMap::DeleteLine(D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	int index = 0;

	// �c����
	if (start.x == end.x)
	{
		// start
		while (1)
		{
			// ���ƉE�̊m�F
			if (m_Map[(int)start.y + index][(int)start.x - 1] == 1 || m_Map[(int)start.y + index][(int)start.x + 1] == 1) { break; }

			// ��ԏ�[�ȊO
			if (start.y + index > 0)
			{
				if (m_Map[(int)start.y - 1 + index][(int)start.x] == 1) { break; }
			}

			// ��������
			m_Map[(int)start.y + index][(int)start.x] = 0;
			index++;
		}

		index = 0;
		// end
		while (1)
		{
			// ���ƉE�̊m�F
			if (m_Map[(int)end.y - index][(int)end.x - 1] == 1 || m_Map[(int)end.y - index][(int)end.x + 1] == 1) { break; }

			// ��ԉ��[�ȊO
			if (end.y - index < (MAP_HEIGHT - 1))
			{
				if (m_Map[(int)end.y + 1 - index][(int)end.x] == 1) { break; }
			}

			// ��������
			m_Map[(int)end.y - index][(int)end.x] = 0;
			index++;
		}

	}
	// ������
	else if (start.y == end.y)
	{
		// start
		while (1)
		{
			// �㉺�̊m�F
			if (m_Map[(int)start.y - 1][(int)start.x + index] == 1 || m_Map[(int)start.y + 1][(int)start.x + index] == 1) { break; }

			// ��ԍ��[�ȊO
			if (start.x + index > 0)
			{
				if (m_Map[(int)start.y][(int)start.x - 1 + index] == 1) { break; }
			}

			// ��������
			m_Map[(int)start.y][(int)start.x + index] = 0;
			index++;
		}

		index = 0;
		// end
		while (1)
		{
			// �㉺�̊m�F
			if (m_Map[(int)end.y - 1][(int)end.x - index] == 1 || m_Map[(int)end.y + 1][(int)end.x - index] == 1) { break; }

			// ��ԉE�[�ȊO
			if (end.x - index < MAP_WIDTH - 1)
			{
				if (m_Map[(int)end.y][(int)end.x + 1 - index] == 1) { break; }
			}

			// ��������
			m_Map[(int)end.y][(int)end.x - index] = 0;
			index++;
		}
	}
}