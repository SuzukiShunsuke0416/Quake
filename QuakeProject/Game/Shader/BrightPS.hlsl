#include "Particle.hlsli"
Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 Blur(float2 uv, int count)
{
	float power = diffuse.y;
	float size = diffuse.z * count;
	float rat = diffuse.w;//��ʔ䒲��

	float4 output = 0;
	for (uint i = 0; i < 9; i++)
	{
		output += tex.Sample(samLinear, uv
			+ float2((float(i % 3) - 1.0f) * size * rat, (float(i / 3) - 1.0f) * size)) * power;
	}

	return output;

}


float4 main(PS_INPUT input) : SV_TARGET
{
	float4 output = 0;

	for (uint i = 1; i <= 6; i++) {
		output += Blur(input.Tex,i);
	}

	float4 BaseColor = tex.Sample(samLinear, input.Tex);

	// ���̕`�揈������������\�������邩
	return lerp(BaseColor, output, step(BaseColor.a, diffuse.x));
}