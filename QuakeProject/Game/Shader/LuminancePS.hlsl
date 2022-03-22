#include "Particle.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 BaseColor = tex.Sample(samLinear, input.Tex);
	float4 output = 0;

	if (length(BaseColor.rgb) > 1.61f) {
		return BaseColor;
	}

	return output;
}