#include"DgCreateMap.h"
#include<math.h>
#include<time.h>
#include<algorithm>
#include<functional>
#include<iostream>

//=====================================================
//  実体化
//=====================================================
int DgCreateMap::m_Map[MAP_HEIGHT][MAP_WIDTH] = { 0 };


//=====================================================================================
// 関数名：Create
// 内容　：フィールド自動生成
// 戻り値：DgCreateMap*
//=====================================================================================
DgCreateMap* DgCreateMap::Create()
{
	// シード生成
	srand((unsigned)time(NULL));
	rand();

	// 縦分割からか横分割からかを決める
	int luck = rand() % 2;
	luck = 1;

	DgCreateMap* map = new DgCreateMap();

	//==============================================
	// 初期分割
	//==============================================
	if (luck == 0)
	{
		float X = static_cast<float>((rand() % (MAP_WIDTH / 2)) + 5);		// 5 ～ ( 5 + WIDTH / 2)の間（縦分割）

		// 縦分割
		DgCreateMap::RegistArea(map, D3DXVECTOR2(0.0f, 0.0f),     D3DXVECTOR2(X - 1.0f, static_cast<float>(MAP_HEIGHT) - 1.0f));
		DgCreateMap::RegistArea(map, D3DXVECTOR2(X + 1.0f, 0.0f), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1.0f, static_cast<float>(MAP_HEIGHT) - 1.0f));
		DgCreateMap::RegistRoot(map, D3DXVECTOR2(X, 0.0f),        D3DXVECTOR2(X, static_cast<float>(MAP_HEIGHT) - 1.0f));

		DgCreateMap::WriteRootToMap(D3DXVECTOR2(X, 0.0f), D3DXVECTOR2(X, MAP_HEIGHT - 1.0f));
	}
	else if (luck == 1)
	{
		float Y = static_cast<float>((rand() % (MAP_HEIGHT / 2)) + 5);		// 5 ～ ( 5 + HEIGHT / 2)-30の間（横分割）
														// 横分割
		DgCreateMap::RegistArea(map, D3DXVECTOR2(static_cast<float>(0), static_cast<float>(0)), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y - 1));
		DgCreateMap::RegistArea(map, D3DXVECTOR2(static_cast<float>(0), Y + 1),					D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, static_cast<float>(MAP_HEIGHT) - 1));
		DgCreateMap::RegistRoot(map, D3DXVECTOR2(static_cast<float>(0), Y),						D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y));

		DgCreateMap::WriteRootToMap(D3DXVECTOR2(static_cast<float>(0), Y), D3DXVECTOR2(static_cast<float>(MAP_WIDTH) - 1, Y));

	}

	//==============================================
	// エリアの分割
	//==============================================
	for (int i = 1; i < (AREA_NUM - 1); ++i)
	{
		luck++;
		luck = luck % 2;

		// 最大マップサイズの検索
		AREA_LIST maxArea = std::for_each(map->areaList.begin(), map->areaList.end(), MaxArea()).GetMaxArea();

		if (maxArea.MaximizeArea->GetWidth() > maxArea.MaximizeArea->GetHeight())
		{
			// 縦分割
			int X = maxArea.MaximizeArea->GetWidth() / 2;
			bool guusuu = (maxArea.MaximizeArea->GetWidth() % 2) == 0;

			// 偶数
			if (guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X + 1), maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - (X - 1), maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->end.y));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x - X, maxArea.MaximizeArea->end.y));

			}
			// 奇数
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
			// 横分割
			int Y = maxArea.MaximizeArea->GetHeight() / 2;
			bool guusuu = (maxArea.MaximizeArea->GetWidth() % 2) == 0;

			// 偶数
			if (guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y - 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - Y));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - Y));

			}
			// 奇数
			else if (!guusuu)
			{
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->start.y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 2)));
				DgCreateMap::RegistArea(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - Y), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y));
				DgCreateMap::RegistRoot(map, D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y + 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));

				DgCreateMap::WriteRootToMap(D3DXVECTOR2(maxArea.MaximizeArea->start.x, maxArea.MaximizeArea->end.y - (Y + 1)), D3DXVECTOR2(maxArea.MaximizeArea->end.x, maxArea.MaximizeArea->end.y - (Y + 1)));

			}
		}

		// 選ばれたエリアの削除
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
	// 部屋を作る
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

		// 壁の厚さを変更
		float random = static_cast<float>(rand() % 2 + 1);
		//float random2 = rand() % 2 + 1;
		D3DXVECTOR2 wallWidth = { random, random };

		DgCreateMap::RegistRoom(map, start + wallWidth, end - wallWidth);
		DgCreateMap::WriteRoomToMap(start + wallWidth, end - wallWidth);
	}

	//==============================================
	// 部屋からrootがあるかチェック
	//==============================================
	for (auto itr = map->roomList.begin(); itr != map->roomList.end(); ++itr)
	{
		int x;
		int y;

		// ルーム内から座標を決める
		(*itr)->ConnectPoint.x = (*itr)->start.x + rand() % (*itr)->GetWidth();
		(*itr)->ConnectPoint.y = (*itr)->start.y + rand() % (*itr)->GetHeight();

		// 上にrootがあるかどうか
		x = static_cast<int>((*itr)->ConnectPoint.x);
		y = static_cast<int>((*itr)->start.y - 1);

		while (1)
		{
			// 存在する
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

		// 下にrootがあるかどうか
		x = static_cast<int>((*itr)->ConnectPoint.x);
		y = static_cast<int>((*itr)->end.y + 1);

		while (1)
		{

			// 存在する
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

		// 左にrootがあるかどうか
		x = static_cast<int>((*itr)->start.x - 1);
		y = static_cast<int>((*itr)->ConnectPoint.y);

		while (1)
		{

			// 存在する
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

		// 右にrootがあるかどうか
		x = static_cast<int>((*itr)->end.x + 1);
		y = static_cast<int>((*itr)->ConnectPoint.y);

		while (1)
		{

			// 存在する
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

		// 部屋から作れるゲートの最大数の算出
		for (int i = 0; i < 4; ++i)
		{
			if ((*itr)->GateExist[i])
			{
				(*itr)->MaxGate++;
			}

		}

		// 部屋のゲートからrootまでを繋げる
		(*itr)->GateNum = 1 + rand() % (*itr)->MaxGate;			// ゲート数をランダムで決める

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
	// 余計な通路を削除する
	//==============================================
	for (auto itr = map->rootList.begin(); itr != map->rootList.end(); ++itr)
	{
		DgCreateMap::DeleteLine((*itr)->start, (*itr)->end);
	}


	//=============================================
	// メモリ解放
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
// 関数名：Release
// 内容　：フィールド解放
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
// 関数名：RegistArea
// 内容　：ダンジョン部屋の領域登録
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
// 関数名：RegistRoot
// 内容　：ダンジョン道のルート登録
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
// 関数名：RegistRoom
// 内容　：ダンジョンエリア内の部屋登録
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
// 関数名：WriteRootToMap
// 内容　：マップに道のルートを書き込む
//=====================================================================================
void DgCreateMap::WriteRootToMap(D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	// 縦分割線
	if (start.x == end.x)
	{
		for (int tate = static_cast<int>(start.y); tate <= static_cast<float>(end.y); tate++)
		{
			m_Map[tate][(int)start.x] = 1;
		}
	}
	// 横分割線
	else if (start.y == end.y)
	{
		for (int yoko = (int)start.x; yoko <= (int)end.x; yoko++)
		{
			m_Map[(int)start.y][yoko] = 1;
		}
	}
}

//=====================================================================================
// 関数名：WriteRoomToMap
// 内容　：マップに部屋を書き込む
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
// 関数名：WriteLoadToMap
// 内容　：部屋に道を書き込む
//=====================================================================================
void DgCreateMap::WriteLoadToMap(D3DXVECTOR2 start, D3DXVECTOR2 end, int mode)
{
	switch (mode)
	{
	case 0:		// 上
		while (1)
		{
			end.y++;
			if (m_Map[(int)end.y][(int)start.x] == 2) { break; }
			m_Map[(int)end.y][(int)start.x] = 1;

		}

		break;
	case 1:		// 下
		while (1)
		{
			end.y--;
			if (m_Map[(int)end.y][(int)start.x] == 2) { break; }
			m_Map[(int)end.y][(int)start.x] = 1;
		}

		break;
	case 2:		// 左
		while (1)
		{
			end.x++;
			if (m_Map[(int)start.y][(int)end.x] == 2) { break; }
			m_Map[(int)start.y][(int)end.x] = 1;
		}

		break;
	case 3:		// 右
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
// 関数名：DeleteLine
// 内容　：余計な道を削除
//=====================================================================================
void DgCreateMap::DeleteLine(D3DXVECTOR2 start, D3DXVECTOR2 end)
{
	int index = 0;

	// 縦分割
	if (start.x == end.x)
	{
		// start
		while (1)
		{
			// 左と右の確認
			if (m_Map[(int)start.y + index][(int)start.x - 1] == 1 || m_Map[(int)start.y + index][(int)start.x + 1] == 1) { break; }

			// 一番上端以外
			if (start.y + index > 0)
			{
				if (m_Map[(int)start.y - 1 + index][(int)start.x] == 1) { break; }
			}

			// 道を消す
			m_Map[(int)start.y + index][(int)start.x] = 0;
			index++;
		}

		index = 0;
		// end
		while (1)
		{
			// 左と右の確認
			if (m_Map[(int)end.y - index][(int)end.x - 1] == 1 || m_Map[(int)end.y - index][(int)end.x + 1] == 1) { break; }

			// 一番下端以外
			if (end.y - index < (MAP_HEIGHT - 1))
			{
				if (m_Map[(int)end.y + 1 - index][(int)end.x] == 1) { break; }
			}

			// 道を消す
			m_Map[(int)end.y - index][(int)end.x] = 0;
			index++;
		}

	}
	// 横分割
	else if (start.y == end.y)
	{
		// start
		while (1)
		{
			// 上下の確認
			if (m_Map[(int)start.y - 1][(int)start.x + index] == 1 || m_Map[(int)start.y + 1][(int)start.x + index] == 1) { break; }

			// 一番左端以外
			if (start.x + index > 0)
			{
				if (m_Map[(int)start.y][(int)start.x - 1 + index] == 1) { break; }
			}

			// 道を消す
			m_Map[(int)start.y][(int)start.x + index] = 0;
			index++;
		}

		index = 0;
		// end
		while (1)
		{
			// 上下の確認
			if (m_Map[(int)end.y - 1][(int)end.x - index] == 1 || m_Map[(int)end.y + 1][(int)end.x - index] == 1) { break; }

			// 一番右端以外
			if (end.x - index < MAP_WIDTH - 1)
			{
				if (m_Map[(int)end.y][(int)end.x + 1 - index] == 1) { break; }
			}

			// 道を消す
			m_Map[(int)end.y][(int)end.x - index] = 0;
			index++;
		}
	}
}