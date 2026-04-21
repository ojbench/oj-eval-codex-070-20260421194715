#!/usr/bin/env python3
import os, sys, json, requests
from datetime import datetime

def main():
    if len(sys.argv) < 3:
        print('Usage: submit_file.py <problem_id> <file_path>')
        sys.exit(1)
    problem_id = int(sys.argv[1])
    file_path = sys.argv[2]
    token = os.environ.get('ACMOJ_TOKEN')
    if not token:
        print('ACMOJ_TOKEN not set')
        sys.exit(1)
    with open(file_path, 'r', encoding='utf-8') as f:
        code = f.read()
    headers = {
        'Authorization': f'Bearer {token}',
        'Content-Type': 'application/x-www-form-urlencoded',
        'User-Agent': 'ACMOJ-Python-Client/2.3'
    }
    data = { 'language': 'c++', 'code': code }
    url = f'https://acm.sjtu.edu.cn/OnlineJudge/api/v1/problem/{problem_id}/submit'
    r = requests.post(url, headers=headers, data=data, timeout=20, proxies={'https': None, 'http': None})
    r.raise_for_status()
    res = r.json()
    print(json.dumps(res))
    if 'id' in res:
        try:
            with open('/workspace/submission_ids.log', 'a') as log:
                log.write(json.dumps({'timestamp': datetime.now().isoformat(sep=' ', timespec='seconds'), 'submission_id': res['id']})+'\n')
            print(f'✅ Submission ID {res[id]} saved to /workspace/submission_ids.log')
        except Exception as e:
            print(f'⚠️ Failed to save submission ID: {e}')

if __name__ == '__main__':
    main()
