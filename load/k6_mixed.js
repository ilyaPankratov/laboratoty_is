import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = { vus: 5, duration: '25s' };

export default function () {
  if (__ITER % 2 === 0) {
    const payload = JSON.stringify({
      title: `mix-${__VU}`,
      assignee: 'pankratov',
      priority: 3,
      effort: 2,
    });
    check(http.post('http://localhost:8080/tasks', payload, {
      headers: { 'Content-Type': 'application/json' },
    }), { posted: (r) => r.status === 201 });
  } else {
    check(http.get('http://localhost:8080/health'), { health: (r) => r.status === 200 });
  }
  sleep(0.5);
}
