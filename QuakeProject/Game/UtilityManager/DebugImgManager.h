/**
 * @file DebugImgManager.h
 * @brief �摜��\�������p�̃}�l�[�W���[
 * @author suzuki.shunsuke
 * @date 2022/03/03
 */

#pragma once
#include "SingletonFounder.h"
#include <SpriteBatch.h>
#include <unordered_map>
#include <vector>
#include <CommonStates.h>

/**
 * @brief ���̃f�o�b�O�C���[�W�̕`��^�C�v
 */
enum class eDebugImgType {
	Full1,
	Full2,
	TridentLogo,

	TYPE_NUM,
};

/**
 * @brief �摜��\�������p�̃}�l�[�W���[
 * @note �O������l(��ށE���W)��n����Draw�֐����Ă΂ꂽ��
 * @note �ꊇ�ŉ摜�`�悷��B
 */
class DebugImgManager :public SingletonFounder<DebugImgManager> {
	friend SingletonFounder;

private:
	/**
	* @brief �R���X�g���N�^
	*/
	DebugImgManager();

	/** �R�����X�e�[�g */
	std::unique_ptr<DirectX::CommonStates> mCommonState;

	/** �X�v���C�g�o�b�` */
	std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	/** �e�N�X�`�� */
	std::unordered_map<eDebugImgType,
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>>mTextures;

	/** �\���ʒu�z�� */
	std::unordered_map <eDebugImgType, std::vector<std::pair<int, int>>>
		mPositions;
public:
	/**
	* @brief �f�X�g���N�^
	*/
	~DebugImgManager() = default;

	/**
	* @brief �`��
	*/
	void Draw();

	/**
	* @brief �ǉ�
	* @param[in] type	�`��^�C�v
	* @param[in] x	�`����W�i�X�N���[��x�j
	* @param[in] y	�`����W�i�X�N���[��y�j
	*/
	void Entry(eDebugImgType type, int x = 0, int y = 0);

	/**
	* @brief �Ă΂ꂽ�u�ԕ`��
	* @param[in] type	�`��^�C�v
	* @param[in] x	�`����W�i�X�N���[��x�j
	* @param[in] y	�`����W�i�X�N���[��y�j
	*/
	void QuickDraw(eDebugImgType type, int x = 0, int y = 0);
private:

	/**
	* @brief �z��f�[�^�̃��Z�b�g
	*/
	void Reset();
};