#include "Particle.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


float4 main(PS_INPUT input) : SV_TARGET
{
	//‰æ‘œ•\Ž¦
	float4 output = tex.Sample(samLinear, input.Tex);

	output.a = step(length(input.Tex - float2(0.5f, 0.5f)),diffuse.x);

	return output;
}