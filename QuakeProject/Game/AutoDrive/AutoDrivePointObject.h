/**
 * @file AutoDrivePointObject.h
 * @brief �������s���Ɏg���|�C���g
 * @author suzuki.shunsuke
 * @date 2022/03/25
 */
#pragma once

 /**
  * @brief �������s���Ɏg���|�C���g
  */
class AutoDrivePointObject {
public:
	enum class Type {
		Start,

		Run,

		Goal,
	};

protected:

	/** ���W */
	DirectX::SimpleMath::Vector3 mPos;

	/** �W�I���g���b�N�v���~�e�B�u */
	std::unique_ptr<DirectX::GeometricPrimitive> mpGeometricPrimitive;
public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] pos	���W
	*/
	AutoDrivePointObject(
		DirectX::SimpleMath::Vector3 pos
	);

	/**
	* @brief �f�X�g���N�^
	*/
	virtual ~AutoDrivePointObject() = default;

	/**
	* @brief �X�V
	*/
	virtual bool Update() = 0;

	/**
	* @brief �`��
	*/
	void Render();

public:

	/**
	* @brief ���W���擾
	*/
	inline const DirectX::SimpleMath::Vector3& GetPos() {
		return mPos;
	}

public:
	/**
	* @brief �Ώە������I�u�W�F�N�g�ɐG�ꂽ��
	*/
	bool IsHitObject();

public:
	/**
	* @brief �I�u�W�F�N�g�|�C���g�𐶐�����
	* @param[in] type	�|�C���g�^�C�v
	* @param[in] pos	���W
	*/
	static AutoDrivePointObject* CreatePointObj(
		Type type,
		DirectX::SimpleMath::Vector3 pos
	);
};

/**
  * @brief �I�[�g�h���C�u�|�C���g�y�X�^�[�g�z
  */
class AutoDrivePoint_Start final :public AutoDrivePointObject {
private:

public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] pos	���W
	*/
	AutoDrivePoint_Start(DirectX::SimpleMath::Vector3 pos) 
		:AutoDrivePointObject(pos)
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDrivePoint_Start() = default;

	/**
	* @brief �X�V
	*/
	bool Update() override;

};

/**
  * @brief �I�[�g�h���C�u�|�C���g�y�����z
  */
class AutoDrivePoint_Run :public AutoDrivePointObject {
private:

public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] pos	���W
	*/
	AutoDrivePoint_Run(DirectX::SimpleMath::Vector3 pos)
		:AutoDrivePointObject(pos)
	{
	}

	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDrivePoint_Run() = default;

	/**
	* @brief �X�V
	*/
	bool Update() override;

};

/**
  * @brief �I�[�g�h���C�u�|�C���g�y�S�[���z
  */
class AutoDrivePoint_Goal :public AutoDrivePointObject {
private:

public:
	/**
	* @brief �R���X�g���N�^
	* @param[in] pos	���W
	*/
	AutoDrivePoint_Goal(DirectX::SimpleMath::Vector3 pos)
		:AutoDrivePointObject(pos)
	{
	}


	/**
	* @brief �f�X�g���N�^
	*/
	~AutoDrivePoint_Goal() = default;

	/**
	* @brief �X�V
	*/
	bool Update() override;

};