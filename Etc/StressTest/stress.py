'''
# Stress Tester
사용법: 같은 폴더의 stress_test_guide.md
'''
import os
import sys
import subprocess

# a를 확장자로 가지는 모든 파일을 사전 순서대로 번호를 매겨서 ext_b로 변경
def rename_files(ext_a, ext_b):
    files = [f for f in os.listdir() if f.endswith(ext_a)]
    
    files.sort()
    
    for index, filename in enumerate(files, start=1):
        new_name = f"{index}.{ext_b}"
        os.rename(filename, new_name)
        print(f"{filename} -> {new_name}")

# import compile_cpp_files
# cpp 파일 일괄 컴파일
def compile_cpp_files():
    # 현재 디렉터리에서 모든 .cpp 파일 검색
    cpp_files = [f for f in os.listdir() if f.endswith(".cpp")]

    for cpp_file in cpp_files:
        # 파일 이름에서 확장자 제거
        file_name = os.path.splitext(cpp_file)[0]
        
        # g++ 컴파일 명령어
        command = ["g++", cpp_file, "-o", file_name]
        # command = ["g++", "-std=c++20", "-DLOCAL", cpp_file, "-o", file_name]
        print(f"Compiling {cpp_file}...")

        # 컴파일 명령어 실행
        result = subprocess.run(command, capture_output=True, text=True)
        
        # 컴파일 에러가 있을 경우 출력
        if result.returncode != 0:
            print(f"Error compiling {cpp_file}:\n{result.stderr}")
        else:
            print(f"{cpp_file} compiled successfully to {file_name}")

# .in 파일을 입력 받아서 .ans 파일과 비교. 없을 경우 file_sol과 file_gen을 이용해 생성
def stress_test(file="a", start=1, file_sol = "", file_gen = "", max_file = -1):
    i = start
    script_dir = os.path.dirname(os.path.abspath(__file__))  # Directory Path
    
    if not file_sol:
        file_sol = f"{file}_sol"
    file_sol = os.path.join(script_dir, file_sol) # Solution or Brute Force

    if not file_gen:
        file_gen = f"{file}_gen"
    file_gen = os.path.join(script_dir, file_gen) # Generator

    sol_flag = os.path.exists(file_sol) or os.path.exists(file_sol+".exe")
    gen_flag = os.path.exists(file_gen) or os.path.exists(file_gen+".exe")
    if not sol_flag:
        print("No Solution File")
    if not gen_flag:
        print("No Generator File")

    while True:
        input_file = os.path.join(script_dir, f"{i}.in") # Input File
        ans_out = os.path.join(script_dir, f"{i}.ans") # Answer File
        my_out = os.path.join(script_dir, f"{i}.ansmy") # My Output File
            
        if not os.path.exists(input_file):
            if not gen_flag:
                print("Every Test Passed!")
                break
            elif os.system(f"{file_gen} {i} > {input_file}"):  # {i} is Seed (Use args)
                print(f"Generator Error Occurred")
                break
        if sol_flag:
            if os.system(f"{file_sol} < {input_file} > {ans_out}"):
                print(f"Solution Error Occurred")
                break
        if not os.path.exists(ans_out):
            print(f"No Output File")
            break
        
        print(f"Testcase {i}")
        if os.system(f"{os.path.join(script_dir, file)} < {input_file} > {my_out}"): 
            print(f"No {file} or Error Occurred")
            break
        if open(my_out).read().rstrip() != open(ans_out).read().rstrip(): # eliminate rstrip if needed
            print("Output is Different")
            break
        
        if i == max_file:
            break
        i += 1
        

if __name__ == "__main__":
    print(f"Usage: python {sys.argv[0]} <file_name> [start_index] [file_sol] [file_gen]")
    
    print("Call rename_files if you want to change file names")
    # rename_files(".in", ".in")
    # rename_files(".out", ".ans")

    print("Call compile_cpp_files if you want to compile all")
    compile_cpp_files()

    file_name = sys.argv[1] if len(sys.argv) > 1 else "a"
    start_index = int(sys.argv[2]) if len(sys.argv) > 2 else 1
    file_sol = sys.argv[3] if len(sys.argv) > 3 else ""
    file_gen = sys.argv[4] if len(sys.argv) > 4 else ""
    
    print("Set max_files = -1 if you want unlimited generation")
    stress_test(file=file_name, start=start_index, file_sol=file_sol, file_gen=file_gen, max_file=10)
