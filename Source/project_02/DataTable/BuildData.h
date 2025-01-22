#pragma once

/**
 * TMap 형태로 데이터 정보 저장
 * Key -> 좌표 정보 값 (ex. FVector가 된다면 x, y, z 좌표를, 아니라면 "(x)_(y)_(z)" 형식의 key로 저장
 * Value -> {
 *		NearWallInfo: 현재 좌표에서 붙어있는 블록들의 정보 값 저장 {
  *		North (북쪽): 좌표 정보 값
  *		South (남쪽): 좌표 정보 값
  *		East (오른쪽): 좌표 정보 값
  *		West (왼쪽): 좌표 정보 값
  *		Ceil (천장): 좌표 정보 값
  *		NorthWall (북쪽): 좌표 정보 값
  *		SouthWall (남쪽): 좌표 정보 값
  *		EastWall (오른쪽): 좌표 정보 값
  *		WestWall (왼쪽): 좌표 정보 값
*		}
*		BuildInfo: 현재 설치된 구조물의 정보 (ex. 벽 재질, 벽 타입 등)
 * }
 *
 * 대략 저렇게 정보를 저장한다. 상황에 따라 Value를 구조체로 변경해 Type을 저장하는 것도 방법으로 보임
 *
 * 아이템 구조물 관련 정보
 * Map 형태로 저장
 * Key: 설치된 순서대로 반영되는 Id 값.
 * Value: BuildInfo 현재 설치된 구조물 정보 (ex. 구조물 타입, 설치된 블록 좌표 정보, 설치된 블록 좌표 기준으로 대략 어느정도 떨어져 있는지 정보)
*/

UENUM()
enum class EBlockPos
{
	North,
	South,
	West,
	East
};

// USTRUCT()
// struct FBuildData
// {
// 	GENERATED_BODY()
//
// 	TMap<EBlockPos, FVector> NearBlockPosInfo;
// };