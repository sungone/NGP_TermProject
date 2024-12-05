#pragma once

class StreamOutputBuffer
{
public:
    void Init(uint64 bufferSize);
    void Bind();
    void UnBind();
    void Render();

    ComPtr<ID3D12Resource> GetBuffer() const { return _StreamOutputBuffer; }
	ComPtr<ID3D12Resource> GetFilledSizeBuffer() const { return _FilledSizeBuffer; }

    D3D12_STREAM_OUTPUT_BUFFER_VIEW GetSOView() const { return _SOBufferView; }

private:
    ComPtr<ID3D12Resource> _StreamOutputBuffer;   // ��Ʈ�� ��� ������ ����
    ComPtr<ID3D12Resource> _FilledSizeBuffer;     // ��µ� ������ ũ�� ������ ����
    ComPtr<ID3D12Resource> _FilledSizeReadbackBuffer;     // ��µ� ������ ũ�� ������ ����
    D3D12_STREAM_OUTPUT_BUFFER_VIEW _SOBufferView;

};