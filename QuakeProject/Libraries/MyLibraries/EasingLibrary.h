#pragma once
#include"pch.h"

/*==========================================================
	【作品で使用するイージング関数集】
	コード拝借サイト → https://easings.net/ja
	※c++用に改変
==========================================================*/
namespace MyEasingLibrary {

	static float easeOutElastic(float x) {
		x = std::max(std::min(1.0f, x), 0.0f);
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;

		return x == 0.0f
			? 0.0f
			: x == 1.0f
			? 1.0f
			: powf(2.0f, -10.0f * x) * sinf((x * 10.0f - 0.75f) * c4) + 1.0f;
	}

	static float easeInElastic(float x){
		x = std::max(std::min(1.0f, x), 0.0f);
		const float c4 = (2.0f * DirectX::XM_PI) / 3.0f;

		return x == 0.0f
		  ? 0.0f
		  : x == 1.0f
		  ? 1.0f
		  : -powf(2.0f, 10.0f * x - 10.0f) * sinf((x * 10.0f - 10.75f) * c4);
	}

	static float  easeInOutElastic(float x){
		const float c5 = (2 * DirectX::XM_PI) / 4.5f;

		return x == 0.0f
		  ? 0.0f
		  : x == 1.0f
		  ? 1.0f
		  : x < 0.5f
		  ? -(powf(2.0f, 20.0f * x - 10.0f) * sin((20.0f * x - 11.125f) * c5)) / 2.0f
		  : (powf(2.0f, -20.0f * x + 10.0f) * sin((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	static float easeInBack(float x){
		x = std::max(std::min(1.0f, x), 0.0f);
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;

		return c3 * x * x * x - c1 * x * x;
	}

	static float easeInOutBack(float x){
		x = std::max(std::min(1.0f, x), 0.0f);
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;

		return x < 0.5f
		  ? (powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f
		  : (powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
	}
}

namespace mel = MyEasingLibrary;