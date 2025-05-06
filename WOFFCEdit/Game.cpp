//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "DisplayObject.h"
#include <string>


using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

Game::Game()

{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    m_deviceResources->RegisterDeviceNotify(this);
	m_displayList.clear();
	
	//initial Settings
	//modes
	m_grid = false;
}

Game::~Game()
{

#ifdef DXTK_AUDIO
    if (m_audEngine)
    {
        m_audEngine->Suspend();
    }
#endif
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	m_Camera = std::make_shared<Camera>();

    m_gamePad = std::make_unique<GamePad>();

    m_keyboard = std::make_unique<Keyboard>();

    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(window);

    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    // Initialize window size
    GetClientRect(window, &m_WindowRect);

#ifdef DXTK_AUDIO
    // Create DirectXTK for Audio objects
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif

    m_audEngine = std::make_unique<AudioEngine>(eflags);

    m_audioEvent = 0;
    m_audioTimerAcc = 10.f;
    m_retryDefault = false;

    m_waveBank = std::make_unique<WaveBank>(m_audEngine.get(), L"adpcmdroid.xwb");

    m_soundEffect = std::make_unique<SoundEffect>(m_audEngine.get(), L"MusicMono_adpcm.wav");
    m_effect1 = m_soundEffect->CreateInstance();
    m_effect2 = m_waveBank->CreateInstance(10);

    m_effect1->Play(true);
    m_effect2->Play();
#endif
}

void Game::SetGridState(bool state)
{
	m_grid = state;
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick(InputCommands *Input)
{
	//copy over the input commands so we have a local version to use elsewhere.
	m_InputCommands = *Input;
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

#ifdef DXTK_AUDIO
    // Only update audio engine once per frame
    if (!m_audEngine->IsCriticalError() && m_audEngine->Update())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
#endif

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    // Pass input commands to camera
    m_Camera->Update(m_InputCommands, timer.GetElapsedSeconds());

    // Determine if mouse hovering over object
    ObjectPickingUpdate();

    if (m_InputCommands.leftMouseDown)
    {
        if (!m_HasExecutedClickFunctionality)
        {
            if (m_IsHovering)
            {
                // Select the hovered object
                m_SelectedObjectID = m_HoveredObjectID;
                m_HasSelected = true;
            }
            else
            {
                // Clicking elsewhere in the world will remove selection so long as object property window is closed
                if (!m_ObjectPropertiesOpen)
                {
                    m_SelectedObjectID = -1;
                    m_HasSelected = false;
                }
            }

            // Prevents more than one click event triggering over frames
            m_HasExecutedClickFunctionality = true;
        }
    }
    else if (!m_InputCommands.leftMouseDown && m_HasExecutedClickFunctionality)
    {
        // Reset mouse functionality for next click
        m_HasExecutedClickFunctionality = false;
    }

    // Focus camera on object, prioritizing hovered over selected
    if (m_InputCommands.focusOnObject && !m_HasFocused)
    {
        if (m_HoveredObjectID != -1)
        {
            m_Camera->m_camPosition = m_displayList[m_HoveredObjectID].m_position;

            m_Camera->m_camPosition.y += 5;
            m_Camera->m_camPosition.x += 5;

            m_Camera->m_camOrientation = DirectX::SimpleMath::Vector3(0, 180, -35);
        }
        else if (m_SelectedObjectID != -1)
        {
            m_Camera->m_camPosition = m_displayList[m_SelectedObjectID].m_position;

            // Repeat logic deliberately as function keeps getting entered even though hover + selected id both equal -1
            m_Camera->m_camPosition.y += 5;
            m_Camera->m_camPosition.x += 5;

            m_Camera->m_camOrientation = DirectX::SimpleMath::Vector3(0, 180, -35);
        }

        m_HasFocused = true;
    }
    else if (!m_InputCommands.focusOnObject && m_HasFocused)
    {
        m_HasFocused = false;
    }


    m_batchEffect->SetView(m_Camera->m_view);
    m_batchEffect->SetWorld(Matrix::Identity);
	m_displayChunk.m_terrainEffect->SetView(m_Camera->m_view);
	m_displayChunk.m_terrainEffect->SetWorld(Matrix::Identity);

#ifdef DXTK_AUDIO
    m_audioTimerAcc -= (float)timer.GetElapsedSeconds();
    if (m_audioTimerAcc < 0)
    {
        if (m_retryDefault)
        {
            m_retryDefault = false;
            if (m_audEngine->Reset())
            {
                // Restart looping audio
                m_effect1->Play(true);
            }
        }
        else
        {
            m_audioTimerAcc = 4.f;

            m_waveBank->Play(m_audioEvent++);

            if (m_audioEvent >= 11)
                m_audioEvent = 0;
        }
    }
#endif
}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    m_grid = true;
    if (m_grid)
    {
        // Draw procedurally generated dynamic grid
        const XMVECTORF32 xaxis = { 512.f, 0.f, 0.f };
        const XMVECTORF32 yaxis = { 0.f, 0.f, 512.f };
        DrawGrid(xaxis, yaxis, g_XMZero, 512, 512, Colors::Gray);
    }

    //bool TestRaytrace = true;
    if (m_IsHovering)
    {
        DrawCubeForDisplayObject(m_HoveredObjectID);
    }
    if (m_HasSelected)
    {
        DrawCubeForDisplayObject(m_SelectedObjectID, FXMVECTOR{ 0, 1, 0, 1 });
    }

    //CAMERA POSITION ON HUD
    m_sprites->Begin();
    WCHAR   Buffer[256];
    std::wstring var = L"Cam X: " + std::to_wstring(m_Camera->m_camPosition.x) + L"Cam Y: " + std::to_wstring(m_Camera->m_camPosition.y) + L"Cam Z: " + std::to_wstring(m_Camera->m_camPosition.z);
    m_font->DrawString(m_sprites.get(), var.c_str(), XMFLOAT2(100, 10), Colors::Green);

    WCHAR Buffer2[256];
    std::wstring var2 = L"Cam Pitch: " + std::to_wstring(m_Camera->m_camOrientation.z) + L"Cam Yaw: " + std::to_wstring(m_Camera->m_camOrientation.y);
    m_font2->DrawString(m_sprites.get(), var2.c_str(), XMFLOAT2(100, 30), Colors::Red);
	m_sprites->End();

	//RENDER OBJECTS FROM SCENEGRAPH
	int numRenderObjects = m_displayList.size();
	for (int i = 0; i < numRenderObjects; i++)
	{
		m_deviceResources->PIXBeginEvent(L"Draw model");
		const XMVECTORF32 scale = { m_displayList[i].m_scale.x, m_displayList[i].m_scale.y, m_displayList[i].m_scale.z };
		const XMVECTORF32 translate = { m_displayList[i].m_position.x, m_displayList[i].m_position.y, m_displayList[i].m_position.z };

		//convert degrees into radians for rotation matrix
		XMVECTOR rotate = Quaternion::CreateFromYawPitchRoll(m_displayList[i].m_orientation.y *3.1415 / 180,
															m_displayList[i].m_orientation.x *3.1415 / 180,
															m_displayList[i].m_orientation.z *3.1415 / 180);

		XMMATRIX local = m_world * XMMatrixTransformation(g_XMZero, Quaternion::Identity, scale, g_XMZero, rotate, translate);

		m_displayList[i].m_model->Draw(context, *m_states, local, m_Camera->m_view, m_Camera->m_projection, false);	//last variable in draw,  make TRUE for wireframe

		m_deviceResources->PIXEndEvent();
	}
    m_deviceResources->PIXEndEvent();

	//RENDER TERRAIN
	context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(m_states->DepthDefault(),0);
	context->RSSetState(m_states->CullNone());
//	context->RSSetState(m_states->Wireframe());		//uncomment for wireframe

	//Render the batch,  This is handled in the Display chunk becuase it has the potential to get complex
	m_displayChunk.RenderBatch(m_deviceResources);

    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetBackBufferRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}

void Game::ObjectPickingUpdate()
{
    // Used to prevent reset of m_IsHovering and m_HoveredObjectID if object collides with ray
    bool collisionOccured = false;

    // Caches the mouse position in near and far plane
    XMVECTOR mouseNearPlane = XMVectorSet(m_InputCommands.MousePos.x, m_InputCommands.MousePos.y, 0, 1);
    XMVECTOR mouseFarPlane = XMVectorSet(m_InputCommands.MousePos.x, m_InputCommands.MousePos.y, 1.f, 1);

    // Cache viewport for projection of mouse coords to world
    D3D11_VIEWPORT viewport = m_deviceResources->GetScreenViewport();

    float closestDistance = 100000.0f;
    float dist = 0;
   
    // Iterate through all objects within the world to determine if we are hovering over an object
    for (int i = 0; i < m_displayList.size(); i++)
    {
        // Create local object matrix based on its translation, scale and rotation and multiply by world matrix
        XMMATRIX objectMatrix = m_world * m_displayList[i].GetObjectMatrix();

        // Unproject mouse coords from screen space to world space
        XMVECTOR nearPoint = XMVector3Unproject(mouseNearPlane, 0.0f, 0.0f, m_WindowRect.right, m_WindowRect.bottom, 
            viewport.MinDepth, viewport.MaxDepth, m_Camera->m_projection,m_Camera->m_view, objectMatrix);

        XMVECTOR farPoint = XMVector3Unproject(mouseFarPlane, 0.0f, 0.0f, m_WindowRect.right, m_WindowRect.bottom,
            viewport.MinDepth, viewport.MaxDepth, m_Camera->m_projection, m_Camera->m_view, objectMatrix);

        // Determine a direction for picking raycast
        XMVECTOR pickingVector = farPoint - nearPoint;
        pickingVector = XMVector3Normalize(pickingVector);
        
        // Iterate through all meshes on model, could have potential meshes. 
        for (int j = 0; j < m_displayList[i].m_model->meshes.size(); j++)
        {
            BoundingBox& meshBounds = m_displayList[i].m_model->meshes[j]->boundingBox;
            if (meshBounds.Intersects(nearPoint, pickingVector, dist))
            {
                m_IsHovering = true;
                collisionOccured = true;

                // This ensures only the closest object is selected as the hovered ID, otherwise objects behind this one with a lower
                // ID will be selected over the closest one.
                if (dist < closestDistance)
                {
                    m_HoveredObjectID = i;
                    closestDistance = dist;
                }
            }
        }
    }

    if (!collisionOccured)
    {
        m_IsHovering = false;
        m_HoveredObjectID = -1;
    }
}

void XM_CALLCONV Game::DrawGrid(FXMVECTOR xAxis, FXMVECTOR yAxis, FXMVECTOR origin, size_t xdivs, size_t ydivs, GXMVECTOR color)
{
    m_deviceResources->PIXBeginEvent(L"Draw grid");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthNone(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);

    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    xdivs = std::max<size_t>(1, xdivs);
    ydivs = std::max<size_t>(1, ydivs);

    for (size_t i = 0; i <= xdivs; ++i)
    {
        float fPercent = float(i) / float(xdivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(xAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, yAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, yAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    for (size_t i = 0; i <= ydivs; i++)
    {
        float fPercent = float(i) / float(ydivs);
        fPercent = (fPercent * 2.0f) - 1.0f;
        XMVECTOR vScale = XMVectorScale(yAxis, fPercent);
        vScale = XMVectorAdd(vScale, origin);

        VertexPositionColor v1(XMVectorSubtract(vScale, xAxis), color);
        VertexPositionColor v2(XMVectorAdd(vScale, xAxis), color);
        m_batch->DrawLine(v1, v2);
    }

    m_batch->End();

    m_deviceResources->PIXEndEvent();
}
void Game::DrawCubeForDisplayObject(int ObjectID, FXMVECTOR Colour)
{
    m_deviceResources->PIXBeginEvent(L"Draw cube");

    auto context = m_deviceResources->GetD3DDeviceContext();
    context->OMSetBlendState(m_states->Opaque(), nullptr, 0xFFFFFFFF);
    context->OMSetDepthStencilState(m_states->DepthDefault(), 0);
    context->RSSetState(m_states->CullCounterClockwise());

    m_batchEffect->Apply(context);
    context->IASetInputLayout(m_batchInputLayout.Get());

    m_batch->Begin();

    // Store bounding box corners within array
    XMFLOAT3 boxCorners[8];
    m_displayList[ObjectID].m_model->meshes[0]->boundingBox.GetCorners(boxCorners);

    // Transform each corner by the object's world matrix
    for (int i = 0; i < 8; ++i)
    {
        XMStoreFloat3(&boxCorners[i], XMVector3Transform(   XMVECTOR{ boxCorners[i].x, boxCorners[i].y, boxCorners[i].z }, 
                                                            m_displayList[ObjectID].GetObjectMatrix()));
    }

    // Store indices of bounding box 
    const int edgeIndices[24] =
    {
        0, 1, 1, 2, 
        2, 3, 3, 0, 

        4, 5, 5, 6, 
        6, 7, 7, 4, 

        0, 4, 1, 5, 
        2, 6, 3, 7  
    };

    // Loop through all indices and draw bounding box lines
    for (int i = 0; i < 24; i++)
    {
        m_batch->DrawLine(
            VertexPositionColor(FXMVECTOR{ 
                boxCorners[edgeIndices[i]].x,
                boxCorners[edgeIndices[i]].y,
                boxCorners[edgeIndices[i]].z }, 
                Colour),

            VertexPositionColor(FXMVECTOR{ 
                boxCorners[edgeIndices[i + 1]].x,
                boxCorners[edgeIndices[i + 1]].y,
                boxCorners[edgeIndices[i + 1]].z }, 
                Colour));

        // Since we draw a line using two indices, need to increment by an extra indice before continuing
        i++;
    }

    m_batch->End();
    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
}

void Game::OnDeactivated()
{
}

void Game::OnSuspending()
{
#ifdef DXTK_AUDIO
    m_audEngine->Suspend();
#endif
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

#ifdef DXTK_AUDIO
    m_audEngine->Resume();
#endif
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();
}

void Game::BuildDisplayList(std::vector<SceneObject> * SceneGraph)
{
	auto device = m_deviceResources->GetD3DDevice();
	auto devicecontext = m_deviceResources->GetD3DDeviceContext();

	if (!m_displayList.empty())		//is the vector empty
	{
		m_displayList.clear();		//if not, empty it
	}

	//for every item in the scenegraph
	int numObjects = SceneGraph->size();
	for (int i = 0; i < numObjects; i++)
	{

		//create a temp display object that we will populate then append to the display list.
		DisplayObject newDisplayObject;
		
		//load model
		std::wstring modelwstr = StringToWCHART(SceneGraph->at(i).model_path);							//convect string to Wchar
		newDisplayObject.m_model = Model::CreateFromCMO(device, modelwstr.c_str(), *m_fxFactory, true);	//get DXSDK to load model "False" for LH coordinate system (maya)

		//Load Texture
		std::wstring texturewstr = StringToWCHART(SceneGraph->at(i).tex_diffuse_path);								//convect string to Wchar
		HRESULT rs;
		rs = CreateDDSTextureFromFile(device, texturewstr.c_str(), nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource

		//if texture fails.  load error default
		if (rs)
		{
			CreateDDSTextureFromFile(device, L"database/data/Error.dds", nullptr, &newDisplayObject.m_texture_diffuse);	//load tex into Shader resource
		}

		//apply new texture to models effect
		newDisplayObject.m_model->UpdateEffects([&](IEffect* effect) //This uses a Lambda function,  if you dont understand it: Look it up.
		{	
			auto lights = dynamic_cast<BasicEffect*>(effect);
			if (lights)
			{
				lights->SetTexture(newDisplayObject.m_texture_diffuse);			
			}
		});

		//set position
		newDisplayObject.m_position.x = SceneGraph->at(i).posX;
		newDisplayObject.m_position.y = SceneGraph->at(i).posY;
		newDisplayObject.m_position.z = SceneGraph->at(i).posZ;
		
		//setorientation
		newDisplayObject.m_orientation.x = SceneGraph->at(i).rotX;
		newDisplayObject.m_orientation.y = SceneGraph->at(i).rotY;
		newDisplayObject.m_orientation.z = SceneGraph->at(i).rotZ;

		//set scale
		newDisplayObject.m_scale.x = SceneGraph->at(i).scaX;
		newDisplayObject.m_scale.y = SceneGraph->at(i).scaY;
		newDisplayObject.m_scale.z = SceneGraph->at(i).scaZ;

		//set wireframe / render flags
		newDisplayObject.m_render		= SceneGraph->at(i).editor_render;
		newDisplayObject.m_wireframe	= SceneGraph->at(i).editor_wireframe;

		newDisplayObject.m_light_type		= SceneGraph->at(i).light_type;
		newDisplayObject.m_light_diffuse_r	= SceneGraph->at(i).light_diffuse_r;
		newDisplayObject.m_light_diffuse_g	= SceneGraph->at(i).light_diffuse_g;
		newDisplayObject.m_light_diffuse_b	= SceneGraph->at(i).light_diffuse_b;
		newDisplayObject.m_light_specular_r = SceneGraph->at(i).light_specular_r;
		newDisplayObject.m_light_specular_g = SceneGraph->at(i).light_specular_g;
		newDisplayObject.m_light_specular_b = SceneGraph->at(i).light_specular_b;
		newDisplayObject.m_light_spot_cutoff = SceneGraph->at(i).light_spot_cutoff;
		newDisplayObject.m_light_constant	= SceneGraph->at(i).light_constant;
		newDisplayObject.m_light_linear		= SceneGraph->at(i).light_linear;
		newDisplayObject.m_light_quadratic	= SceneGraph->at(i).light_quadratic;
		
		m_displayList.push_back(newDisplayObject);
		
	}
		
		
		
}

void Game::BuildDisplayChunk(ChunkObject * SceneChunk)
{
	//populate our local DISPLAYCHUNK with all the chunk info we need from the object stored in toolmain
	//which, to be honest, is almost all of it. Its mostly rendering related info so...
	m_displayChunk.PopulateChunkData(SceneChunk);		//migrate chunk data
	m_displayChunk.LoadHeightMap(m_deviceResources);
	m_displayChunk.m_terrainEffect->SetProjection(m_Camera->m_projection);
	m_displayChunk.InitialiseBatch();
}

void Game::SaveDisplayChunk(ChunkObject * SceneChunk)
{
	m_displayChunk.SaveHeightMap();			//save heightmap to file.
}

void Game::SpawnNewDisplayObject(DisplayObject newObject)
{
    // Set model to same as first model in vector, as well as initializing texture
    newObject.m_model = m_displayList[0].m_model;
    DirectX::CreateDDSTextureFromFile(m_deviceResources->GetD3DDevice(), L"database/data/placeholder.dds", nullptr, &newObject.m_texture_diffuse);

    // Update effects 
    newObject.m_model->UpdateEffects([&](DirectX::IEffect* effect)
    {
        auto lights = dynamic_cast<DirectX::BasicEffect*>(effect);
        if (lights)
        {
           lights->SetTexture(newObject.m_texture_diffuse);
        }
    });

    // Apply texture diffuse based on other object
    newObject.m_texture_diffuse = m_displayList[0].m_texture_diffuse;

    // Reset ID's for all objects
    m_displayList.push_back(newObject);
    for (int i = 0; i < m_displayList.size(); i++)
    {
        m_displayList[i].m_ID = i;
    }
}

void Game::DeleteSelectedDisplayObject()
{
    // Erase selected object
    if (m_SelectedObjectID != -1)
    {
        m_displayList.erase(m_displayList.begin() + m_SelectedObjectID);

        // Clear freed memory so that new ID's are not generated for non existing objects
        m_displayList.shrink_to_fit();

        // Reset ID's for remaining objects
        for (int i = 0; i < m_displayList.size(); i++)
        {
            m_displayList[i].m_ID = i;
        }
    }

    // Reset selected object after deletion
    m_SelectedObjectID = -1;
    m_HasSelected = false;
}

#ifdef DXTK_AUDIO
void Game::NewAudioDevice()
{
    if (m_audEngine && !m_audEngine->IsAudioDevicePresent())
    {
        // Setup a retry in 1 second
        m_audioTimerAcc = 1.f;
        m_retryDefault = true;
    }
}
#endif


#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto device = m_deviceResources->GetD3DDevice();

    m_states = std::make_unique<CommonStates>(device);

    m_fxFactory = std::make_unique<EffectFactory>(device);
	m_fxFactory->SetDirectory(L"database/data/"); //fx Factory will look in the database directory
	m_fxFactory->SetSharing(false);	//we must set this to false otherwise it will share effects based on the initial tex loaded (When the model loads) rather than what we will change them to.

    m_sprites = std::make_unique<SpriteBatch>(context);

    m_batch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(context);

    m_batchEffect = std::make_unique<BasicEffect>(device);
    m_batchEffect->SetVertexColorEnabled(true);

    {
        void const* shaderByteCode;
        size_t byteCodeLength;

        m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

        DX::ThrowIfFailed(
            device->CreateInputLayout(VertexPositionColor::InputElements,
                VertexPositionColor::InputElementCount,
                shaderByteCode, byteCodeLength,
                m_batchInputLayout.ReleaseAndGetAddressOf())
        );
    }

    m_font = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

    m_font2 = std::make_unique<SpriteFont>(device, L"SegoeUI_18.spritefont");

//    m_shape = GeometricPrimitive::CreateTeapot(context, 4.f, 8);

    // SDKMESH has to use clockwise winding with right-handed coordinates, so textures are flipped in U
    m_model = Model::CreateFromSDKMESH(device, L"tiny.sdkmesh", *m_fxFactory);
	

    // Load textures
    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"seafloor.dds", nullptr, m_texture1.ReleaseAndGetAddressOf())
    );

    DX::ThrowIfFailed(
        CreateDDSTextureFromFile(device, L"windowslogo.dds", nullptr, m_texture2.ReleaseAndGetAddressOf())
    );

}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    auto size = m_deviceResources->GetOutputSize();
    float aspectRatio = float(size.right) / float(size.bottom);
    float fovAngleY = 70.0f * XM_PI / 180.0f;

    // This is a simple example of change that can be made when the app is in
    // portrait or snapped view.
    if (aspectRatio < 1.0f)
    {
        fovAngleY *= 2.0f;
    }

    // This sample makes use of a right-handed coordinate system using row-major matrices.
    m_Camera->UpdateProjectionView(aspectRatio, fovAngleY);

    m_batchEffect->SetProjection(m_Camera->m_projection);

    // Resize window when client window changes
    m_WindowRect = size;
}

void Game::OnDeviceLost()
{
    m_states.reset();
    m_fxFactory.reset();
    m_sprites.reset();
    m_batch.reset();
    m_batchEffect.reset();
    m_font.reset();
    m_shape.reset();
    m_model.reset();
    m_texture1.Reset();
    m_texture2.Reset();
    m_batchInputLayout.Reset();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion

std::wstring StringToWCHART(std::string s)
{

	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}
