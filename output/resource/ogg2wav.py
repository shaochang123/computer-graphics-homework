import os
import argparse
from glob import glob
from pydub import AudioSegment

def convert_ogg_to_wav(ogg_file, output_dir=None):
    """
    将单个 OGG 文件转换为 WAV 格式
    
    Args:
        ogg_file: OGG 文件路径
        output_dir: 输出目录(可选)，如果不指定则保存在原目录
    
    Returns:
        生成的 WAV 文件路径
    """
    try:
        # 获取文件名和目录
        base_name = os.path.basename(ogg_file)
        file_name = os.path.splitext(base_name)[0]
        
        # 决定输出路径
        if output_dir:
            os.makedirs(output_dir, exist_ok=True)
            output_file = os.path.join(output_dir, f"{file_name}.wav")
        else:
            dir_name = os.path.dirname(ogg_file)
            output_file = os.path.join(dir_name, f"{file_name}.wav")
        
        # 加载 OGG 文件
        print(f"转换 {ogg_file}...")
        audio = AudioSegment.from_file(ogg_file, format="ogg")
        
        # 导出为 WAV
        audio.export(output_file, format="wav")
        print(f"已保存为 {output_file}")
        
        return output_file
    
    except Exception as e:
        print(f"转换 {ogg_file} 失败: {str(e)}")
        return None

def batch_convert(directory, output_dir=None, recursive=False):
    """
    批量转换目录下所有 OGG 文件
    
    Args:
        directory: 源目录
        output_dir: 输出目录(可选)
        recursive: 是否递归处理子目录
    """
    # 确定搜索模式
    if recursive:
        search_pattern = os.path.join(directory, "**", "*.ogg")
        ogg_files = glob(search_pattern, recursive=True)
    else:
        search_pattern = os.path.join(directory, "*.ogg")
        ogg_files = glob(search_pattern)
    
    # 检查是否找到文件
    if not ogg_files:
        print(f"在 {directory} 中没有找到 OGG 文件")
        return
    
    # 转换所有找到的文件
    converted_count = 0
    for ogg_file in ogg_files:
        if convert_ogg_to_wav(ogg_file, output_dir):
            converted_count += 1
    
    print(f"共转换了 {converted_count} 个文件")

def main():
   convert_ogg_to_wav("resource/switch.ogg")

if __name__ == "__main__":
    # 检查依赖
    try:
        import pydub
    except ImportError:
        print("错误: 需要安装 pydub 库")
        print("请运行: pip install pydub")
        exit(1)
        
    # 运行主程序
    main()