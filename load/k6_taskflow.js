import http from 'k6/http';
import { check, sleep } from 'k6';

export const options = {
  vus: 8,
  duration: '30s',
  thresholds: {
    http_req_failed: ['rate<0.01'],
    http_req_duration: ['p(95)<400'],
  },
};

export default function () {
  const payload = JSON.stringify({
    title: `pn-${__VU}-${__ITER}`,
    assignee: 'pankratov',
    priority: 2,
    effort: 3,
  });
  check(http.post('http://localhost:8080/tasks', payload, {
    headers: { 'Content-Type': 'application/json' },
  }), { created: (r) => r.status === 201 });
  check(http.get('http://localhost:8080/tasks'), { listed: (r) => r.status === 200 });
  sleep(0.6);
}
