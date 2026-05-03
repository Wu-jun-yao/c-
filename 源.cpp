#include<graphics.h>
#include<iostream>
#include<stdio.h>
#include<easyx.h>
#include<windows.h>
#include<vector>
#include<cmath>
#include <mmsystem.h>
#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib,"winmm.lib")
using namespace std;

// 全局变量与资源声明
IMAGE bk;
IMAGE btn;
IMAGE victoryBg;
int mx;
int my;

// 游戏状态
enum GameState
{
	MENU,
	GAME,
	VICTORY
};
GameState gameState = MENU;

struct Monster;

// 剑气结构体
struct Slash
{
	int x;
	int y;
	int speed;
	bool isRight;
	IMAGE* img;
	bool active;
};

// 弓箭结构体
struct Arrow
{
	int x;
	int y;
	int speed;
	bool isRight;
	IMAGE* img;
	bool active;
	int damage;
};

// 玩家结构体
struct Player
{
	int x;
	int y;
	int width;
	int height;
	int speed;
	bool isRight;

	bool isWalkFrame;
	int animTimer;
	int animInterval;

	bool isAttacking;
	int attackTimer;
	int attackFrame;
	int attackInterval;

	float vy;
	float gravity;
	int jumpForce;
	bool isOnGround;
	int jumpCount;
	int maxJumpCount;

	int maxHp;
	int currentHp;

	IMAGE imgRightIdle;
	IMAGE imgRightWalk;
	IMAGE imgRightAtk1;
	IMAGE imgRightAtk2;
	IMAGE imgRightJump;
	IMAGE imgRightHurt;
	IMAGE imgRightSlash;

	IMAGE imgLeftIdle;
	IMAGE imgLeftWalk;
	IMAGE imgLeftAtk1;
	IMAGE imgLeftAtk2;
	IMAGE imgLeftJump;
	IMAGE imgLeftHurt;
	IMAGE imgLeftSlash;

	bool isSliding;
	int slideTimer;
	int slideDuration;
	int slideSpeed;
	IMAGE imgRightSlide;
	IMAGE imgLeftSlide;

	bool isHurt;
	int hurtTimer;
	int hurtInterval;
	int invincibleTimer;
	int invincibleInterval;

	bool isCastingSlash;
	int castTimer;
	int castDuration;

	// 奔跑相关
	bool isRunning;
	int runFrame;
	int runTimer;
	int runInterval;
	DWORD lastADKeyTime;
	int runSpeed;

	// 突刺相关
	bool isDashing;
	int dashTimer;
	int dashDuration;
	int dashSpeed;
	bool dashDirRight;

	// 眩晕相关
	bool isStunned;
	int stunTimer;
	int stunDuration;
	IMAGE imgRightStunned;
	IMAGE imgLeftStunned;

	// 图片资源
	IMAGE imgRightRun1;
	IMAGE imgRightRun2;
	IMAGE imgLeftRun1;
	IMAGE imgLeftRun2;
	IMAGE imgRightDash;
	IMAGE imgLeftDash;
};

// 怪物结构体
struct Monster
{
	int x;
	int y;
	int width;
	int height;
	int speed;
	bool isRight;

	bool isWalkFrame;
	int animTimer;
	int animInterval;

	int maxHp;
	int currentHp;

	bool isHit;
	int hitTimer;
	int hitInterval;

	int hurtCDTimer;
	const int hurtCDInterval = 15;

	bool isAttacking;
	int attackTimer;
	int attackInterval;
	int attackRange;
	int damage;
	bool isRanged;
	bool hasShot;

	IMAGE* pImgRightWalk1;
	IMAGE* pImgRightWalk2;
	IMAGE* pImgLeftWalk1;
	IMAGE* pImgLeftWalk2;
	IMAGE* pImgRightHit;
	IMAGE* pImgLeftHit;
	IMAGE* pImgRightAtk;
	IMAGE* pImgLeftAtk;
	IMAGE* pImgRightAtk2;
	IMAGE* pImgLeftAtk2;
};

// Boss专属结构体
struct Boss
{
	int x;
	int y;
	int width;
	int height;
	int speed;
	bool isRight;

	bool isWalkFrame;
	int animTimer;
	int animInterval;

	int maxHp;
	int currentHp;

	bool isHit;
	int hitTimer;
	int hitInterval;

	int hurtCDTimer;
	const int hurtCDInterval = 15;

	bool isAttacking;
	int attackTimer;
	int attackInterval;
	int attackRange;
	int damage;

	// Boss吼叫技能
	bool isRoaring;
	int roarTimer;
	int roarDuration;
	int roarCooldown;
	int roarCooldownMax;
	float roarRange;
	IMAGE imgRightRoar;
	IMAGE imgLeftRoar;

	IMAGE* pImgRightWalk1;
	IMAGE* pImgRightWalk2;
	IMAGE* pImgLeftWalk1;
	IMAGE* pImgLeftWalk2;
	IMAGE* pImgRightHit;
	IMAGE* pImgLeftHit;
	IMAGE* pImgRightAtk;
	IMAGE* pImgLeftAtk;
};

Player player;
IMAGE gameBg;
IMAGE gameBg2;
IMAGE portalImg;

IMAGE slashRight;
IMAGE slashLeft;

vector<Slash> slashes;
const int MAX_SLASHES = 15;

vector<Arrow> arrows;
IMAGE arrowRight;
IMAGE arrowLeft;

const int groundY = 500;

// 怪物资源
IMAGE monsterRightWalk1;
IMAGE monsterRightWalk2;
IMAGE monsterLeftWalk1;
IMAGE monsterLeftWalk2;
IMAGE monsterRightHit;
IMAGE monsterLeftHit;
IMAGE monsterRightAtk;
IMAGE monsterLeftAtk;

IMAGE monster2RightWalk1;
IMAGE monster2RightWalk2;
IMAGE monster2LeftWalk1;
IMAGE monster2LeftWalk2;
IMAGE monster2RightHit;
IMAGE monster2LeftHit;
IMAGE monster2RightAtk;
IMAGE monster2LeftAtk;

IMAGE monster3RightWalk1;
IMAGE monster3RightWalk2;
IMAGE monster3LeftWalk1;
IMAGE monster3LeftWalk2;
IMAGE monster3RightHit;
IMAGE monster3LeftHit;
IMAGE monster3RightAtk1;
IMAGE monster3RightAtk2;
IMAGE monster3LeftAtk1;
IMAGE monster3LeftAtk2;

// Boss资源
IMAGE bossRightWalk1;
IMAGE bossRightWalk2;
IMAGE bossLeftWalk1;
IMAGE bossLeftWalk2;
IMAGE bossRightHit;
IMAGE bossLeftHit;
IMAGE bossRightAtk;
IMAGE bossLeftAtk;
IMAGE bossRightRoar;
IMAGE bossLeftRoar;

vector<Monster> monsters;
const int kMaxMonsters = 10;
int spawnedCount = 0;
int spawnTimer = 0;
const int kSpawnInterval = 120;

Boss boss;
bool bossActive = false;

int currentMap = 1;
bool portalActive = false;
int portalX = 1150;
int portalY = groundY;

const int VICTORY_BTN_X = 490;
const int VICTORY_BTN_Y = 500;

int lastMap = -1;

//函数声明
void loadRes();
void initGame();
void initMonsters();
void switchToMap(int mapId);
void drawHealthBar();
void drawMonsterHealthBar(Monster& m);
void drawBossHealthBar();
bool checkAllMonstersDead();
bool contain(int rx, int ry, int mouse_x, int mouse_y);
void drawPng(int x, int y, IMAGE* img);
bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
void processAttackDamage();
void processDashCollision();
void updateBoss();
void updateMonsters();
void drawMonsters();
void drawBoss();
void controlPlayer();
void draw();
void mouseEvent();
void processMonsterAttack();
void updateSlashes();
void drawSlashes();
void spawnSlash();
void updateArrows();
void drawArrows();
void spawnArrow(Monster& m);
void playMapBGM(int mapId);
void stopAllMusic();

// 函数实现

void playMapBGM(int mapId) {
	stopAllMusic();

	if (mapId == 1) {
		mciSendString("open music1.mp3 alias music1", NULL, 0, NULL);
		mciSendString("play music1 repeat", NULL, 0, NULL);
	}
	else if (mapId == 2) {
		mciSendString("open music2.mp3 alias music2", NULL, 0, NULL);
		mciSendString("play music2 repeat", NULL, 0, NULL);
	}
}

void stopAllMusic() {
	mciSendString("stop music1", NULL, 0, NULL);
	mciSendString("close music1", NULL, 0, NULL);
	mciSendString("stop music2", NULL, 0, NULL);
	mciSendString("close music2", NULL, 0, NULL);
}

void loadRes()
{
	loadimage(&bk, "./Res/bk.png", getwidth(), getheight());
	loadimage(&btn, "./Res/btn.png", 300, 70);
	loadimage(&gameBg, "./Res/game_bk.png", 1280, 720);
	loadimage(&gameBg2, "./Res/game_bk2.png", 1280, 720);
	loadimage(&portalImg, "./Res/portal.png", 100, 100);
	loadimage(&victoryBg, "./Res/victory_bg.png", 1280, 720);

	// 剑气图片
	loadimage(&slashRight, "./Res/slash_right.png", 60, 60);
	loadimage(&slashLeft, "./Res/slash_left.png", 60, 60);

	// 弓箭图片
	loadimage(&arrowRight, "./Res/arrow_right.png", 80, 40);
	loadimage(&arrowLeft, "./Res/arrow_left.png", 80, 40);

	// 玩家
	loadimage(&player.imgRightIdle, "./Res/player_right_idle.png", 100, 120);
	loadimage(&player.imgRightWalk, "./Res/player_right_walk.png", 100, 120);
	loadimage(&player.imgLeftIdle, "./Res/player_left_idle.png", 100, 120);
	loadimage(&player.imgLeftWalk, "./Res/player_left_walk.png", 100, 120);
	loadimage(&player.imgRightAtk1, "./Res/player_right_atk1.png", 100, 120);
	loadimage(&player.imgRightAtk2, "./Res/player_right_atk2.png", 100, 120);
	loadimage(&player.imgLeftAtk1, "./Res/player_left_atk1.png", 100, 120);
	loadimage(&player.imgLeftAtk2, "./Res/player_left_atk2.png", 100, 120);
	loadimage(&player.imgRightJump, "./Res/player_right_jump.png", 100, 120);
	loadimage(&player.imgLeftJump, "./Res/player_left_jump.png", 100, 120);
	loadimage(&player.imgRightSlide, "./Res/player_right_slide.png", 100, 120);
	loadimage(&player.imgLeftSlide, "./Res/player_left_slide.png", 100, 120);
	loadimage(&player.imgRightHurt, "./Res/player_right_hurt.png", 100, 120);
	loadimage(&player.imgLeftHurt, "./Res/player_left_hurt.png", 100, 120);
	loadimage(&player.imgRightSlash, "./Res/player_right_slash.png", 100, 120);
	loadimage(&player.imgLeftSlash, "./Res/player_left_slash.png", 100, 120);

	// 玩家眩晕图
	loadimage(&player.imgRightStunned, "./Res/player_right_stunned.png", 100, 120);
	loadimage(&player.imgLeftStunned, "./Res/player_left_stunned.png", 100, 120);

	// 玩家奔跑图
	loadimage(&player.imgRightRun1, "./Res/player_right_run1.png", 100, 120);
	loadimage(&player.imgRightRun2, "./Res/player_right_run2.png", 100, 120);
	loadimage(&player.imgLeftRun1, "./Res/player_left_run1.png", 100, 120);
	loadimage(&player.imgLeftRun2, "./Res/player_left_run2.png", 100, 120);

	// 玩家突刺图
	loadimage(&player.imgRightDash, "./Res/player_right_dash.png", 100, 120);
	loadimage(&player.imgLeftDash, "./Res/player_left_dash.png", 100, 120);

	// 怪物1
	loadimage(&monsterRightWalk1, "./Res/monster_right_walk1.png", 120, 90);
	loadimage(&monsterRightWalk2, "./Res/monster_right_walk2.png", 120, 90);
	loadimage(&monsterLeftWalk1, "./Res/monster_left_walk1.png", 120, 90);
	loadimage(&monsterLeftWalk2, "./Res/monster_left_walk2.png", 120, 90);
	loadimage(&monsterRightHit, "./Res/monster_right_hit.png", 120, 90);
	loadimage(&monsterLeftHit, "./Res/monster_left_hit.png", 120, 90);
	loadimage(&monsterRightAtk, "./Res/monster_right_atk.png", 120, 90);
	loadimage(&monsterLeftAtk, "./Res/monster_left_atk.png", 120, 90);

	// 怪物2
	loadimage(&monster2RightWalk1, "./Res/monster2_right_walk1.png", 120, 90);
	loadimage(&monster2RightWalk2, "./Res/monster2_right_walk2.png", 120, 90);
	loadimage(&monster2LeftWalk1, "./Res/monster2_left_walk1.png", 120, 90);
	loadimage(&monster2LeftWalk2, "./Res/monster2_left_walk2.png", 120, 90);
	loadimage(&monster2RightHit, "./Res/monster2_right_hit.png", 120, 90);
	loadimage(&monster2LeftHit, "./Res/monster2_left_hit.png", 120, 90);
	loadimage(&monster2RightAtk, "./Res/monster2_right_atk.png", 120, 90);
	loadimage(&monster2LeftAtk, "./Res/monster2_left_atk.png", 120, 90);

	// 怪物3
	loadimage(&monster3RightWalk1, "./Res/monster3_right_walk1.png", 120, 90);
	loadimage(&monster3RightWalk2, "./Res/monster3_right_walk2.png", 120, 90);
	loadimage(&monster3LeftWalk1, "./Res/monster3_left_walk1.png", 120, 90);
	loadimage(&monster3LeftWalk2, "./Res/monster3_left_walk2.png", 120, 90);
	loadimage(&monster3RightHit, "./Res/monster3_right_hit.png", 120, 90);
	loadimage(&monster3LeftHit, "./Res/monster3_left_hit.png", 120, 90);
	loadimage(&monster3RightAtk1, "./Res/monster3_right_atk1.png", 120, 90);
	loadimage(&monster3RightAtk2, "./Res/monster3_right_atk2.png", 120, 90);
	loadimage(&monster3LeftAtk1, "./Res/monster3_left_atk1.png", 120, 90);
	loadimage(&monster3LeftAtk2, "./Res/monster3_left_atk2.png", 120, 90);

	// Boss
	loadimage(&bossRightWalk1, "./Res/boss_right_walk1.png", 200, 160);
	loadimage(&bossRightWalk2, "./Res/boss_right_walk2.png", 200, 160);
	loadimage(&bossLeftWalk1, "./Res/boss_left_walk1.png", 200, 160);
	loadimage(&bossLeftWalk2, "./Res/boss_left_walk2.png", 200, 160);
	loadimage(&bossRightHit, "./Res/boss_right_hit.png", 200, 160);
	loadimage(&bossLeftHit, "./Res/boss_left_hit.png", 200, 160);
	loadimage(&bossRightAtk, "./Res/boss_right_atk.png", 200, 160);
	loadimage(&bossLeftAtk, "./Res/boss_left_atk.png", 200, 160);
	// Boss吼叫图
	loadimage(&bossRightRoar, "./Res/boss_right_roar.png", 200, 160);
	loadimage(&bossLeftRoar, "./Res/boss_left_roar.png", 200, 160);
}

void drawHealthBar() {
	const int barWidth = 80;
	const int barHeight = 8;
	int barX = player.x + (player.width - barWidth) / 2;
	int barY = player.y - 15;

	setfillcolor(RGB(200, 50, 50));
	fillrectangle(barX, barY, barX + barWidth, barY + barHeight);

	float hpPercent = (float)player.currentHp / player.maxHp;
	int currentBarWidth = (int)(barWidth * hpPercent);

	setfillcolor(RGB(50, 200, 50));
	fillrectangle(barX, barY, barX + currentBarWidth, barY + barHeight);

	setlinecolor(BLACK);
	rectangle(barX, barY, barX + barWidth, barY + barHeight);
}

void drawMonsterHealthBar(Monster& m) {
	const int barWidth = 60;
	const int barHeight = 6;
	int barX = m.x + (m.width - barWidth) / 2;
	int barY = m.y - 12;

	setfillcolor(RGB(150, 30, 30));
	fillrectangle(barX, barY, barX + barWidth, barY + barHeight);

	float hpPercent = (float)m.currentHp / m.maxHp;
	int currentBarWidth = (int)(barWidth * hpPercent);
	if (currentBarWidth < 0) currentBarWidth = 0;

	setfillcolor(RGB(30, 150, 30));
	fillrectangle(barX, barY, barX + currentBarWidth, barY + barHeight);

	setlinecolor(BLACK);
	rectangle(barX, barY, barX + barWidth, barY + barHeight);
}

void drawBossHealthBar() {
	if (!bossActive || currentMap != 2) return;

	const int screenWidth = 1280;
	const int barWidth = 600;
	const int barHeight = 30;
	int barX = (screenWidth - barWidth) / 2;
	int barY = 30;

	setfillcolor(BLACK);
	fillrectangle(barX - 2, barY - 2, barX + barWidth + 2, barY + barHeight + 2);
	setfillcolor(RGB(100, 20, 20));
	fillrectangle(barX, barY, barX + barWidth, barY + barHeight);

	float hpPercent = (float)boss.currentHp / boss.maxHp;
	if (hpPercent < 0) hpPercent = 0;
	int currentBarWidth = (int)(barWidth * hpPercent);

	setfillcolor(RGB(220, 20, 20));
	fillrectangle(barX, barY, barX + currentBarWidth, barY + barHeight);
	setlinecolor(WHITE);
	rectangle(barX, barY, barX + barWidth, barY + barHeight);

	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, "黑体");
	outtextxy(barX - 50, barY + 2, "BOSS");
}

bool checkAllMonstersDead()
{
	if (spawnedCount < kMaxMonsters) return false;
	for (auto& m : monsters)
	{
		if (m.currentHp > 0) return false;
	}
	return true;
}

bool checkMap2AllCleared()
{
	if (currentMap != 2) return false;
	if (!checkAllMonstersDead()) return false;
	if (bossActive && boss.currentHp > 0) return false;
	return true;
}

void switchToMap(int mapId)
{
	currentMap = mapId;
	portalActive = false;
	monsters.clear();
	spawnedCount = 0;
	spawnTimer = 0;
	slashes.clear();
	arrows.clear();

	player.x = 100;
	player.y = groundY;

	if (currentMap == 2)
	{
		bossActive = true;
		boss.x = 1000;
		boss.y = groundY - 40;
		boss.width = 200;
		boss.height = 160;
		boss.speed = 2;
		boss.isRight = false;

		boss.isWalkFrame = false;
		boss.animTimer = 0;
		boss.animInterval = 10;
		boss.maxHp = 500;
		boss.currentHp = 500;
		boss.isHit = false;
		boss.hitTimer = 0;
		boss.hitInterval = 20;
		boss.hurtCDTimer = boss.hurtCDInterval;

		boss.pImgRightWalk1 = &bossRightWalk1;
		boss.pImgRightWalk2 = &bossRightWalk2;
		boss.pImgLeftWalk1 = &bossLeftWalk1;
		boss.pImgLeftWalk2 = &bossLeftWalk2;
		boss.pImgRightHit = &bossRightHit;
		boss.pImgLeftHit = &bossLeftHit;
		boss.pImgRightAtk = &bossRightAtk;
		boss.pImgLeftAtk = &bossLeftAtk;

		// Boss吼叫初始化
		boss.isRoaring = false;
		boss.roarTimer = 0;
		boss.roarDuration = 60;
		boss.roarCooldown = 0;
		boss.roarCooldownMax = 600;
		boss.roarRange = 300.0f;

		boss.isAttacking = false;
		boss.attackTimer = 0;
		boss.attackInterval = 50;
		boss.attackRange = 160;
		boss.damage = 16;
	}
	else
	{
		bossActive = false;
	}
}

void initMonsters()
{
	monsters.clear();
	spawnedCount = 0;
	spawnTimer = 0;
	portalActive = false;
	bossActive = false;
	arrows.clear();
}

void initGame()
{
	player.x = 100;
	player.y = groundY;
	player.width = 100;
	player.height = 120;
	player.speed = 5;
	player.isRight = true;

	player.maxHp = 200;
	player.currentHp = 200;

	player.isWalkFrame = false;
	player.animTimer = 0;
	player.animInterval = 8;

	player.isAttacking = false;
	player.attackTimer = 0;
	player.attackFrame = 0;
	player.attackInterval = 4;

	player.vy = 0;
	player.gravity = 0.8f;
	player.jumpForce = -18;
	player.isOnGround = true;
	player.jumpCount = 0;
	player.maxJumpCount = 2;

	player.isSliding = false;
	player.slideTimer = 0;
	player.slideDuration = 22;
	player.slideSpeed = 14;

	player.isHurt = false;
	player.hurtTimer = 0;
	player.hurtInterval = 20;
	player.invincibleTimer = 0;
	player.invincibleInterval = 70;

	player.isCastingSlash = false;
	player.castTimer = 0;
	player.castDuration = 10;

	// 眩晕初始化
	player.isStunned = false;
	player.stunTimer = 0;
	player.stunDuration = 60;

	// 奔跑初始化
	player.isRunning = false;
	player.runFrame = 0;
	player.runTimer = 0;
	player.runInterval = 8;
	player.lastADKeyTime = 0;
	player.runSpeed = 9;

	// 突刺初始化
	player.isDashing = false;
	player.dashTimer = 0;
	player.dashDuration = 15;
	player.dashSpeed = 15;

	currentMap = 1;
	initMonsters();
}

bool contain(int rx, int ry, int mouse_x, int mouse_y)
{
	return (mouse_x > rx && mouse_x < rx + 300 && mouse_y > ry && mouse_y < ry + 50);
}

void drawPng(int x, int y, IMAGE* img)
{
	HDC dstDC = GetImageHDC(NULL);
	HDC srcDc = GetImageHDC(img);
	int w = img->getwidth();
	int h = img->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER,0,255,AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDc, 0, 0, w, h, bf);
}

bool checkCollision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2)
{
	return x1 < x2 + w2 && x1 + w1 > x2 && y1 < y2 + h2 && y1 + h1 > y2;
}

void spawnSlash()
{
	Slash s;
	s.x = player.x + (player.isRight ? player.width : -40);
	s.y = player.y + 30;
	s.speed = player.isRight ? 15 : -15;
	s.isRight = player.isRight;
	s.img = player.isRight ? &slashRight : &slashLeft;
	s.active = true;
	slashes.push_back(s);
}

void spawnArrow(Monster& m)
{
	Arrow a;
	a.x = m.x + (m.isRight ? m.width : -50);
	a.y = m.y + 30;
	a.speed = m.isRight ? 10 : -10;
	a.isRight = m.isRight;
	a.img = m.isRight ? &arrowRight : &arrowLeft;
	a.active = true;
	a.damage = m.damage;
	arrows.push_back(a);
}

void updateSlashes()
{
	for (auto& s : slashes)
	{
		if (!s.active) continue;
		s.x += s.speed;

		if (s.x < -100 || s.x > 1400)
		{
			s.active = false;
			continue;
		}

		for (auto& m : monsters)
		{
			if (m.currentHp <= 0) continue;
			if (checkCollision(s.x, s.y, 60, 40, m.x, m.y, m.width, m.height))
			{
				if (m.hurtCDTimer >= m.hurtCDInterval)
				{
					m.currentHp -= 15;
					m.hurtCDTimer = 0;
					m.isHit = true;
					m.hitTimer = 0;
					s.active = false;
				}
			}
		}

		if (bossActive && boss.currentHp > 0)
		{
			if (checkCollision(s.x, s.y, 60, 40, boss.x, boss.y, boss.width, boss.height))
			{
				if (boss.hurtCDTimer >= boss.hurtCDInterval)
				{
					boss.currentHp -= 15;
					boss.hurtCDTimer = 0;
					boss.isHit = true;
					boss.hitTimer = 0;
					s.active = false;
				}
			}
		}
	}

	for (auto& s : slashes)
	{
		if (!s.active) continue;
		for (auto& a : arrows)
		{
			if (!a.active) continue;
			if (checkCollision(s.x, s.y, 40, 30, a.x, a.y, 40, 15))
			{
				s.active = false;
				a.active = false;
			}
		}
	}

	for (auto it = slashes.begin(); it != slashes.end();)
	{
		if (!it->active) it = slashes.erase(it);
		else it++;
	}
}

void updateArrows()
{
	for (auto& a : arrows)
	{
		if (!a.active) continue;
		a.x += a.speed;

		if (a.x < -100 || a.x > 1400)
		{
			a.active = false;
			continue;
		}

		if (!player.isSliding && player.invincibleTimer <= 0)
		{
			if (checkCollision(a.x, a.y, 50, 20, player.x, player.y, player.width, player.height))
			{
				player.currentHp -= a.damage;
				if (player.currentHp < 0) player.currentHp = 0;
				player.isHurt = true;
				player.hurtTimer = 0;
				player.invincibleTimer = player.invincibleInterval;
				if (player.x > a.x) player.x += 20; else player.x -= 20;
				a.active = false;
			}
		}
	}

	for (auto it = arrows.begin(); it != arrows.end();)
	{
		if (!it->active) it = arrows.erase(it);
		else it++;
	}
}

void drawSlashes()
{
	for (auto& s : slashes)
	{
		if (s.active) drawPng(s.x, s.y, s.img);
	}
}

void drawArrows()
{
	for (auto& a : arrows)
	{
		if (a.active) drawPng(a.x, a.y, a.img);
	}
}

void processAttackDamage()
{
	if (!player.isAttacking || player.attackFrame != 1) return;

	int attackRangeX = player.x;
	int attackRangeW = player.width;
	if (player.isRight) attackRangeW += 30;
	else { attackRangeX -= 30; attackRangeW += 30; }

	for (auto& m : monsters)
	{
		if (m.currentHp <= 0 || m.hurtCDTimer < m.hurtCDInterval) continue;
		if (checkCollision(attackRangeX, player.y, attackRangeW, player.height, m.x, m.y, m.width, m.height))
		{
			m.currentHp -= 10;
			m.hurtCDTimer = 0;
			m.isHit = true;
			m.hitTimer = 0;
			if (player.isRight) m.x += 10; else m.x -= 10;
		}
	}

	if (bossActive && boss.currentHp > 0 && boss.hurtCDTimer >= boss.hurtCDInterval)
	{
		if (checkCollision(attackRangeX, player.y, attackRangeW, player.height, boss.x, boss.y, boss.width, boss.height))
		{
			boss.currentHp -= 10;
			boss.hurtCDTimer = 0;
			boss.isHit = true;
			boss.hitTimer = 0;
			if (player.isRight) boss.x += 15; else boss.x -= 15;
		}
	}
}

void processDashCollision()
{
	if (!player.isDashing) return;

	int dashX = player.x - 10;
	int dashY = player.y;
	int dashW = player.width + 20;
	int dashH = player.height;

	for (auto& m : monsters)
	{
		if (m.currentHp <= 0) continue;
		if (checkCollision(dashX, dashY, dashW, dashH, m.x, m.y, m.width, m.height))
		{
			m.x += player.dashDirRight ? 40 : -40;
			m.currentHp -= 8;
			m.isHit = true;
			m.hitTimer = 0;
		}
	}
}

void updateBoss()
{
	if (!bossActive || currentMap != 2 || boss.currentHp <= 0) return;

	// Boss吼叫技能逻辑
	if (boss.roarCooldown > 0) boss.roarCooldown--;

	if (boss.isRoaring)
	{
		boss.roarTimer++;
		// 吼叫第1帧时检查范围并眩晕玩家
		if (boss.roarTimer == 1)
		{
			float dx = (player.x + player.width / 2) - (boss.x + boss.width / 2);
			float dy = (player.y + player.height / 2) - (boss.y + boss.height / 2);
			float dist = sqrt(dx * dx + dy * dy);

			if (dist < boss.roarRange && !player.isStunned)
			{
				player.isStunned = true;
				player.stunTimer = 0;
				player.isRunning = false;
				player.isDashing = false;
				player.isAttacking = false;
				player.isSliding = false;
				player.isCastingSlash = false;
			}
		}

		if (boss.roarTimer >= boss.roarDuration)
		{
			boss.isRoaring = false;
			boss.roarTimer = 0;
		}
		return;
	}

	// 冷却结束后随机触发吼叫
	if (boss.roarCooldown <= 0 && !boss.isHit && !boss.isAttacking)
	{
		if (rand() % 100 == 0)
		{
			boss.isRoaring = true;
			boss.roarTimer = 0;
			boss.roarCooldown = boss.roarCooldownMax;
			return;
		}
	}

	// 原有Boss逻辑
	if (boss.hurtCDTimer < boss.hurtCDInterval) boss.hurtCDTimer++;
	if (boss.isHit) { boss.hitTimer++; if (boss.hitTimer >= boss.hitInterval) boss.isHit = false; return; }
	if (boss.isAttacking) { boss.attackTimer++; if (boss.attackTimer >= 18) { boss.isAttacking = false; boss.attackTimer = 0; } return; }
	if (player.x > boss.x) { boss.x += boss.speed; boss.isRight = true; }
	else if (player.x < boss.x) { boss.x -= boss.speed; boss.isRight = false; }
	boss.animTimer++; if (boss.animTimer >= boss.animInterval) { boss.animTimer = 0; boss.isWalkFrame = !boss.isWalkFrame; }
}

void updateMonsters()
{
	if (gameState != GAME) return;

	if (currentMap == 2 && !portalActive && checkMap2AllCleared()) {
		portalActive = true;
	}

	if (spawnedCount < kMaxMonsters)
	{
		spawnTimer++;
		if (spawnTimer >= kSpawnInterval)
		{
			spawnTimer = 0;
			Monster m;
			m.x = 1280;
			m.y = groundY + 20;
			m.width = 120;
			m.height = 90;
			m.speed = 1 + rand() % 2;
			m.isRight = false;
			m.isWalkFrame = false;
			m.animTimer = 0;
			m.animInterval = 12;
			m.isHit = false;
			m.hitTimer = 0;
			m.hitInterval = 30;
			m.hurtCDTimer = m.hurtCDInterval;
			m.isAttacking = false;
			m.attackTimer = 0;
			m.attackInterval = 70;
			m.attackRange = 85;
			m.damage = 8;
			m.isRanged = false;
			m.hasShot = false;

			if (currentMap == 1) {
				m.maxHp = 50; m.currentHp = 50;
				m.pImgRightWalk1 = &monsterRightWalk1; m.pImgRightWalk2 = &monsterRightWalk2;
				m.pImgLeftWalk1 = &monsterLeftWalk1; m.pImgLeftWalk2 = &monsterLeftWalk2;
				m.pImgRightHit = &monsterRightHit; m.pImgLeftHit = &monsterLeftHit;
				m.pImgRightAtk = &monsterRightAtk; m.pImgLeftAtk = &monsterLeftAtk;
			}
			else {
				int monsterType = rand() % 2;
				if (monsterType == 0) {
					m.maxHp = 80; m.currentHp = 80;
					m.pImgRightWalk1 = &monster2RightWalk1; m.pImgRightWalk2 = &monster2RightWalk2;
					m.pImgLeftWalk1 = &monster2LeftWalk1; m.pImgLeftWalk2 = &monster2LeftWalk2;
					m.pImgRightHit = &monster2RightHit; m.pImgLeftHit = &monster2LeftHit;
					m.pImgRightAtk = &monster2RightAtk; m.pImgLeftAtk = &monster2LeftAtk;
				}
				else {
					m.maxHp = 80; m.currentHp = 80;
					m.isRanged = true;
					m.attackRange = 400;
					m.pImgRightWalk1 = &monster3RightWalk1; m.pImgRightWalk2 = &monster3RightWalk2;
					m.pImgLeftWalk1 = &monster3LeftWalk1; m.pImgLeftWalk2 = &monster3LeftWalk2;
					m.pImgRightHit = &monster3RightHit; m.pImgLeftHit = &monster3LeftHit;
					m.pImgRightAtk = &monster3RightAtk1;
					m.pImgLeftAtk = &monster3LeftAtk1;
					m.pImgRightAtk2 = &monster3RightAtk2;
					m.pImgLeftAtk2 = &monster3LeftAtk2;
				}
			}
			monsters.push_back(m);
			spawnedCount++;
		}
	}

	if (currentMap == 1 && !portalActive && checkAllMonstersDead()) portalActive = true;

	for (auto& m : monsters)
	{
		if (m.currentHp <= 0) continue;
		if (m.hurtCDTimer < m.hurtCDInterval) m.hurtCDTimer++;
		if (m.isHit) { m.hitTimer++; if (m.hitTimer >= m.hitInterval) m.isHit = false; continue; }

		if (m.isAttacking)
		{
			m.attackTimer++;
			if (m.isRanged)
			{
				if (m.attackTimer == 15 && !m.hasShot)
				{
					spawnArrow(m);
					m.hasShot = true;
				}
				if (m.attackTimer >= 45)
				{
					m.isAttacking = false;
					m.attackTimer = 0;
					m.hasShot = false;
				}
			}
			else
			{
				if (m.attackTimer >= 16) { m.isAttacking = false; m.attackTimer = 0; }
			}
			continue;
		}

		int dist = abs(player.x - m.x);

		if (m.isRanged)
		{
			if (dist < 200)
			{
				if (player.x > m.x) { m.x -= m.speed; m.isRight = false; }
				else { m.x += m.speed; m.isRight = true; }
			}
			else if (dist > 350)
			{
				if (player.x > m.x) { m.x += m.speed; m.isRight = true; }
				else { m.x -= m.speed; m.isRight = false; }
			}
		}
		else
		{
			if (player.x > m.x) { m.x += m.speed; m.isRight = true; }
			else if (player.x < m.x) { m.x -= m.speed; m.isRight = false; }
		}

		m.animTimer++; if (m.animTimer >= m.animInterval) { m.animTimer = 0; m.isWalkFrame = !m.isWalkFrame; }
	}
}

void drawMonsters()
{
	if (gameState != GAME) return;
	for (auto& m : monsters)
	{
		if (m.currentHp <= 0) continue;
		IMAGE* drawImg = NULL;

		if (m.isAttacking)
		{
			if (m.isRanged)
			{
				if (m.attackTimer < 15)
					drawImg = m.isRight ? m.pImgRightAtk : m.pImgLeftAtk;
				else
					drawImg = m.isRight ? m.pImgRightAtk2 : m.pImgLeftAtk2;
			}
			else
			{
				drawImg = m.isRight ? m.pImgRightAtk : m.pImgLeftAtk;
			}
		}
		else if (m.isHit) drawImg = m.isRight ? m.pImgRightHit : m.pImgLeftHit;
		else drawImg = m.isRight ? (m.isWalkFrame ? m.pImgRightWalk2 : m.pImgRightWalk1) : (m.isWalkFrame ? m.pImgLeftWalk2 : m.pImgLeftWalk1);

		if (drawImg) drawPng(m.x, m.y, drawImg);
		drawMonsterHealthBar(m);
	}
}

void drawBoss()
{
	if (!bossActive || currentMap != 2 || boss.currentHp <= 0) return;
	IMAGE* drawImg = NULL;

	if (boss.isRoaring)
		drawImg = boss.isRight ? &bossRightRoar : &bossLeftRoar;
	else if (boss.isAttacking)
		drawImg = boss.isRight ? boss.pImgRightAtk : boss.pImgLeftAtk;
	else if (boss.isHit)
		drawImg = boss.isRight ? boss.pImgRightHit : boss.pImgLeftHit;
	else
		drawImg = boss.isRight ? (boss.isWalkFrame ? boss.pImgRightWalk2 : boss.pImgRightWalk1) : (boss.isWalkFrame ? boss.pImgLeftWalk2 : boss.pImgLeftWalk1);

	if (drawImg) drawPng(boss.x, boss.y, drawImg);
}

void controlPlayer()
{
	if (gameState != GAME) return;

	// 眩晕逻辑（最高优先级）
	if (player.isStunned)
	{
		player.stunTimer++;
		player.vy += player.gravity;
		player.y += (int)player.vy;
		if (player.y >= groundY) {
			player.y = groundY;
			player.vy = 0;
			player.isOnGround = true;
			player.jumpCount = 0;
		}
		if (player.stunTimer >= player.stunDuration)
		{
			player.isStunned = false;
			player.stunTimer = 0;
		}
		return;
	}

	// 突刺逻辑
	if (player.isDashing)
	{
		player.dashTimer++;
		player.x += player.dashDirRight ? player.dashSpeed : -player.dashSpeed;
		player.x = max(0, min(player.x, 1280 - player.width));
		if (player.dashTimer >= player.dashDuration) player.isDashing = false;
		return;
	}

	// 双击A/D 奔跑
	static bool aPressedLast = false, dPressedLast = false;
	bool aPressed = GetAsyncKeyState('A') & 0x8000;
	bool dPressed = GetAsyncKeyState('D') & 0x8000;
	DWORD now = GetTickCount();
	const int DOUBLE_CLICK_GAP = 300;

	if (aPressed && !aPressedLast)
	{
		if (now - player.lastADKeyTime < DOUBLE_CLICK_GAP && !player.isRight)
			player.isRunning = true;
		player.lastADKeyTime = now;
		player.isRight = false;
	}
	if (dPressed && !dPressedLast)
	{
		if (now - player.lastADKeyTime < DOUBLE_CLICK_GAP && player.isRight)
			player.isRunning = true;
		player.lastADKeyTime = now;
		player.isRight = true;
	}
	aPressedLast = aPressed;
	dPressedLast = dPressed;

	if (!aPressed && !dPressed) player.isRunning = false;

	// 奔跑中按 J 突刺
	static bool jPressedLast = false;
	bool jPressed = GetAsyncKeyState('J') & 0x8000;
	if (jPressed && !jPressedLast && player.isRunning)
	{
		player.isDashing = true;
		player.dashTimer = 0;
		player.dashDirRight = player.isRight;
		player.isRunning = false;
	}
	jPressedLast = jPressed;

	// 剑气 U
	static bool uPressed = false;
	if (GetAsyncKeyState('U') & 0x8000)
	{
		if (!uPressed && !player.isSliding && !player.isAttacking && !player.isDashing)
		{
			uPressed = true;
			player.isCastingSlash = true;
			player.castTimer = 0;
			spawnSlash();
		}
	}
	else uPressed = false;

	if (player.isCastingSlash)
	{
		player.castTimer++;
		if (player.castTimer >= player.castDuration) player.isCastingSlash = false;
		return;
	}

	// 滑行 L
	static bool lKeyPressed = false;
	if (GetAsyncKeyState('L') & 0x8000)
	{
		if (!lKeyPressed && !player.isSliding && player.isOnGround && !player.isAttacking && !player.isDashing)
		{
			lKeyPressed = true;
			player.isSliding = true;
			player.slideTimer = 0;
		}
	}
	else lKeyPressed = false;

	if (player.isSliding)
	{
		player.slideTimer++;
		player.x += player.isRight ? player.slideSpeed : -player.slideSpeed;
		player.x = max(0, min(player.x, 1280 - player.width));
		if (player.slideTimer >= player.slideDuration) player.isSliding = false;
		return;
	}

	// 传送 F
	static bool fKeyPressed = false;
	if (GetAsyncKeyState('F') & 0x8000)
	{
		if (!fKeyPressed && portalActive)
		{
			fKeyPressed = true;
			if (currentMap == 1) switchToMap(2);
			else if (currentMap == 2) gameState = VICTORY;
		}
	}
	else fKeyPressed = false;

	// 攻击 J
	if (GetAsyncKeyState('J') & 0x8000)
	{
		if (!player.isAttacking && !player.isCastingSlash && !player.isDashing && !player.isSliding)
		{
			player.isAttacking = true;
			player.attackTimer = 0;
			player.attackFrame = 0;
		}
	}

	if (player.isAttacking)
	{
		player.attackTimer++;
		if (player.attackTimer >= player.attackInterval)
		{
			player.attackTimer = 0;
			player.attackFrame++;
			if (player.attackFrame > 2) { player.isAttacking = false; player.attackFrame = 0; }
		}
		return;
	}

	// 奔跑时可以跳跃
	static bool kKeyPressed = false;
	if (GetAsyncKeyState('K') & 0x8000)
	{
		if (!kKeyPressed && player.jumpCount < player.maxJumpCount)
		{
			kKeyPressed = true;
			player.vy = (float)player.jumpForce;
			player.jumpCount++;
			player.isOnGround = false;
		}
	}
	else kKeyPressed = false;

	// 重力
	player.vy += player.gravity;
	player.y += (int)player.vy;
	if (player.y >= groundY) {
		player.y = groundY;
		player.vy = 0;
		player.isOnGround = true;
		player.jumpCount = 0;
	}

	// 空中保持奔跑速度
	bool isMoving = false;
	if (player.isRunning)
	{
		player.x += player.isRight ? player.runSpeed : -player.runSpeed;
		isMoving = true;

		if (player.isOnGround)
		{
			player.runTimer++;
			if (player.runTimer >= player.runInterval)
			{
				player.runTimer = 0;
				player.runFrame = !player.runFrame;
			}
		}
	}
	else
	{
		if (GetAsyncKeyState('A') & 0x8000) { player.x -= player.speed; player.isRight = false; isMoving = true; }
		if (GetAsyncKeyState('D') & 0x8000) { player.x += player.speed; player.isRight = true; isMoving = true; }
	}

	player.x = max(0, min(player.x, 1280 - player.width));

	// 走路动画
	if (!player.isRunning && isMoving && player.isOnGround)
	{
		player.animTimer++;
		if (player.animTimer >= player.animInterval) { player.animTimer = 0; player.isWalkFrame = !player.isWalkFrame; }
	}
	else if (!player.isRunning) player.isWalkFrame = false;
}

void processMonsterAttack()
{
	if (player.isSliding) return;
	if (player.invincibleTimer > 0) player.invincibleTimer--;
	if (player.isHurt) { player.hurtTimer++; if (player.hurtTimer >= player.hurtInterval) player.isHurt = false; }

	auto doAttack = [&](Monster& m) {
		if (m.currentHp <= 0 || m.isHit || m.isAttacking) return;
		int dist = abs(player.x - m.x);
		m.attackTimer++;
		m.isRight = (player.x > m.x);
		if (m.attackTimer >= m.attackInterval && dist < m.attackRange)
		{
			m.isAttacking = true;
			m.attackTimer = 0;
			if (!m.isRanged && player.invincibleTimer <= 0)
			{
				player.currentHp -= m.damage;
				if (player.currentHp < 0) player.currentHp = 0;
				player.isHurt = true;
				player.hurtTimer = 0;
				player.invincibleTimer = player.invincibleInterval;
				if (player.x > m.x) player.x += 20; else player.x -= 20;
			}
		}
		};

	for (auto& m : monsters) doAttack(m);
	if (bossActive)
	{
		if (!boss.isRoaring)
		{
			if (boss.currentHp <= 0 || boss.isHit || boss.isAttacking) return;
			int dist = abs(player.x - boss.x);
			boss.attackTimer++;
			boss.isRight = (player.x > boss.x);
			if (boss.attackTimer >= boss.attackInterval && dist < boss.attackRange)
			{
				boss.isAttacking = true;
				boss.attackTimer = 0;
				if (player.invincibleTimer <= 0)
				{
					player.currentHp -= boss.damage;
					if (player.currentHp < 0) player.currentHp = 0;
					player.isHurt = true;
					player.hurtTimer = 0;
					player.invincibleTimer = player.invincibleInterval;
					if (player.x > boss.x) player.x += 20; else player.x -= 20;
				}
			}
		}
	}
}

void draw()
{
	if (gameState == MENU)
	{
		putimage(0, 0, &bk);
		settextcolor(RGB(54, 53, 51));
		settextstyle(30, 0, "宋体");
		outtextxy(100, 50, "月圆之夜");
		int th = textheight("月圆之夜");
		setfillcolor(RGB(54, 53, 51));
		solidrectangle(0, th + 55, 200, 60 + th);
		settextstyle(20, 0, "宋体");
		outtextxy(100, 70 + th, "大作业");
		char arr[][20] = { "继续游戏","新游戏","载入游戏","前尘往事","小曲","设置","退出" };
		int ty = 200;
		settextcolor(RGB(134, 130, 123));
		settextstyle(20, 0, "华文楷体");
		for (int i = 0; i < 7; ++i)
		{
			if (contain(100, ty + 50 * i, mx, my)) { drawPng(50, ty + i * 50 - 25, &btn); settextcolor(BLACK); }
			outtextxy(100, ty + 50 * i, arr[i]);
			settextcolor(WHITE);
		}
	}
	else if (gameState == VICTORY)
	{
		putimage(0, 0, &victoryBg);
		if (contain(VICTORY_BTN_X, VICTORY_BTN_Y, mx, my)) {
			drawPng(VICTORY_BTN_X - 20, VICTORY_BTN_Y - 10, &btn);
		}
		stopAllMusic();
	}
	else if (gameState == GAME)
	{
		putimage(0, 0, currentMap == 1 ? &gameBg : &gameBg2);

		if (lastMap != currentMap) {
			playMapBGM(currentMap);
			lastMap = currentMap;
		}

		if (player.isRight)
		{
			if (player.isStunned) drawPng(player.x, player.y, &player.imgRightStunned);
			else if (player.isDashing) drawPng(player.x, player.y, &player.imgRightDash);
			else if (player.isRunning) drawPng(player.x, player.y, player.runFrame ? &player.imgRightRun2 : &player.imgRightRun1);
			else if (player.isCastingSlash) drawPng(player.x, player.y, &player.imgRightSlash);
			else if (player.isHurt) drawPng(player.x, player.y, &player.imgRightHurt);
			else if (player.isSliding) drawPng(player.x, player.y, &player.imgRightSlide);
			else if (player.isAttacking) {
				if (player.attackFrame == 0) drawPng(player.x, player.y, &player.imgRightIdle);
				else if (player.attackFrame == 1) drawPng(player.x, player.y, &player.imgRightAtk1);
				else if (player.attackFrame == 2) drawPng(player.x, player.y, &player.imgRightAtk2);
			}
			else if (!player.isOnGround) drawPng(player.x, player.y, &player.imgRightJump);
			else drawPng(player.x, player.y, player.isWalkFrame ? &player.imgRightWalk : &player.imgRightIdle);
		}
		else
		{
			if (player.isStunned) drawPng(player.x, player.y, &player.imgLeftStunned);
			else if (player.isDashing) drawPng(player.x, player.y, &player.imgLeftDash);
			else if (player.isRunning) drawPng(player.x, player.y, player.runFrame ? &player.imgLeftRun2 : &player.imgLeftRun1);
			else if (player.isCastingSlash) drawPng(player.x, player.y, &player.imgLeftSlash);
			else if (player.isHurt) drawPng(player.x, player.y, &player.imgLeftHurt);
			else if (player.isSliding) drawPng(player.x, player.y, &player.imgLeftSlide);
			else if (player.isAttacking) {
				if (player.attackFrame == 0) drawPng(player.x, player.y, &player.imgLeftIdle);
				else if (player.attackFrame == 1) drawPng(player.x, player.y, &player.imgLeftAtk1);
				else if (player.attackFrame == 2) drawPng(player.x, player.y, &player.imgLeftAtk2);
			}
			else if (!player.isOnGround) drawPng(player.x, player.y, &player.imgLeftJump);
			else drawPng(player.x, player.y, player.isWalkFrame ? &player.imgLeftWalk : &player.imgLeftIdle);
		}

		drawHealthBar();
		drawMonsters();
		drawBoss();
		drawSlashes();
		drawArrows();
		drawBossHealthBar();

		if (portalActive)
		{
			drawPng(portalX, portalY, &portalImg);
			settextcolor(YELLOW);
			settextstyle(20, 0, "宋体");
			outtextxy(portalX, portalY - 30, "按 F 传送");
		}
	}
}

void mouseEvent()
{
	ExMessage msg;
	while (peekmessage(&msg, EX_MOUSE))
	{
		mx = msg.x; my = msg.y;
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (gameState == MENU)
			{
				if (contain(100, 250, mx, my)) { initGame(); gameState = GAME; }
				if (contain(100, 500, mx, my)) { closegraph(); exit(0); }
			}
			else if (gameState == VICTORY)
			{
				if (contain(VICTORY_BTN_X, VICTORY_BTN_Y, mx, my)) {
					closegraph();
					exit(0);
				}
			}
		}
	}
}

int main()
{
	initgraph(1280, 720);
	loadRes();
	setbkmode(TRANSPARENT);
	BeginBatchDraw();
	const int FPS = 40;
	const int frameDelay = 1000 / FPS;
	DWORD frameStart;

	while (true)
	{
		frameStart = GetTickCount();
		cleardevice();
		mouseEvent();
		controlPlayer();
		processAttackDamage();
		processDashCollision();
		updateMonsters();
		updateBoss();
		updateSlashes();
		updateArrows();
		processMonsterAttack();
		draw();
		FlushBatchDraw();

		// ========== 稳定帧率修复 ==========
		int cost = GetTickCount() - frameStart;
		int sleepTime = frameDelay - cost;
		if (sleepTime > 0)
		{
			Sleep(sleepTime);
		}
		else
		{
			Sleep(1); // 音乐导致的超速必加这行
		}
	}
	closegraph();
	return 0;
}