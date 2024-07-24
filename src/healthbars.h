#pragma once

#include <d3dx9math.h>
#include "primrender.h"
#include <vector>
#include <map>
#include <memory>

enum class VehicleClass : unsigned int {
	CAR = 0x336fcacf,
	CHOPPER = 0xb80933aa
};

struct CopCarInfo {
	D3DXVECTOR3 position;
	float health;
	float healthAnimation;
	float healthReduceCountdown;
	float deathTimer;
	float deathReduceSpeed;
	float disabledTimer;
};

struct HealthBarDraw {
	D3DXVECTOR4 drawPosViewSpace;
	D3DXCOLOR borderColor;
	D3DXCOLOR healthBarColor;
	D3DXCOLOR healthAnimationColor;

	float health;
	float healthAnimation;

	bool operator<(HealthBarDraw& rhs) {
		// Used to sort healthbars by depth.
		// Using ">" to sort in descending order instead of ascending order.		
		return drawPosViewSpace.z > rhs.drawPosViewSpace.z;
	}
};

class HealthbarRenderer {
public:
	void Draw();

private:
	void UpdateState();
	void UpdateCopCars();

	bool isPaused;
	float simDeltaTime;
	float timeScale;
	float currentSimTime;
	float lastSimTime = -1;
	float pursuitActiveCounter = 0;
	bool pursuitActive = false;
	bool gameMomentCamEnabled = false;
	bool inCopFreezeCam = false;

	D3DXMATRIX* viewMat;
	D3DXMATRIX* viewProjMat;	
	D3DXMATRIX projMat;

	std::map<void*, CopCarInfo> copCars;
	std::vector<HealthBarDraw> healthBars;
	PrimitiveRenderer primitiveRenderer;

};
