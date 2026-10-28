import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = { vus: 6, duration: '20s' };

export default function () {
  check(http.get('http://localhost:8080/health'), { ok: (r) => r.status === 200 });
  check(http.get('http://localhost:8080/tasks'), { list: (r) => r.status === 200 });
  sleep(0.4);
}
