#pragma once
#include "CRenderComponent.h"

#include "CTexture.h"
#include "CStructuredBuffer.h"

class CTileMap :
    public CRenderComponent
{
private:
    Ptr<CTexture>       m_pAtlasTex;
    Vec2                m_vSlicePixel;
    Vec2                m_vSliceUV;

    UINT                m_iRowCount;
    UINT                m_iColCount;   

    UINT                m_iTileCountX;
    UINT                m_iTileCountY;
    vector<tTileData>   m_vecTileData;
    CStructuredBuffer*  m_pBuffer;

    bool                m_bBufferUpdated;


public:
    void SetAtlasTex(Ptr<CTexture> _pAtlasTex) 
    { 
        m_pAtlasTex = _pAtlasTex; 
        m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileSize(Vec2 _vPixelSize) 
    {  
        m_vSlicePixel = _vPixelSize; 
        if(nullptr != m_pAtlasTex)
            m_vSliceUV = m_vSlicePixel / Vec2(m_pAtlasTex->Width(), m_pAtlasTex->Height());
    }

    void SetTileMapCount(UINT _iCountX, UINT _iCountY);
    void SetTileData(int _iTileIdx, int _iImgIdx);
    void ClearTileData();

public:
    Ptr<CTexture> GetTexture() { return m_pAtlasTex; }

    Vec2 GetSlicePixel() { return m_vSlicePixel; }
    void SetSlicePixel(Vec2 _v) { m_vSlicePixel = _v; }
    Vec2 GetSliceUV() { return m_vSliceUV; }
    void SetSliceUV(Vec2 _v) { m_vSliceUV = _v; }

    UINT GetRowCount() { return m_iRowCount; }
    void SetRowCount(UINT _i) { m_iRowCount = _i; }
    UINT GetColCount() { return m_iColCount; }
    void SetColCount(UINT _i) { m_iColCount = _i; }

    UINT GetTileCountX() { return m_iTileCountX; }
    void SetTileCountX(UINT _i) { m_iTileCountX = _i; }
    UINT GetTileCountY() { return m_iTileCountY; }
    void SetTileCountY(UINT _i) { m_iTileCountY = _i; }

    const vector<tTileData>& GetTileData() { return m_vecTileData; }
    void SetVecTileData(const vector<tTileData> _v) { m_vecTileData = _v; }

public:
    virtual void finalupdate() override;
    virtual void render() override;
    virtual void UpdateData() override;

public:
    virtual void SaveToScene(FILE* _pFile) override;
    virtual void LoadFromScene(FILE* _pFile) override;
    CLONE(CTileMap)

public:
    CTileMap();
    CTileMap(const CTileMap& _origin);
    ~CTileMap();
};

