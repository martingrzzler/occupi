const staticCacheName = 'site-cache-v5';
const assets = ['/fallback.html'];


self.addEventListener('install', async e => {
  const set = async () => {
    const cache = await caches.open(staticCacheName);
    cache.addAll(assets);
  }

  e.waitUntil(set());
});

self.addEventListener('activate', e => {
  const work = async () => {
    const keys = await caches.keys();
    return Promise.all(
      keys
      .filter(key => key !== staticCacheName)
      .map(key => caches.delete(key))
    )
  }

  e.waitUntil(work());
});

self.addEventListener('fetch', e => {
  const get = async () => {
   const res = await caches.match(e.request);
   return res || fetch(e.request).catch(() => {
    if(e.request.url.indexOf('.html') > -1) return caches.match('/fallback.html');
   });
  }
  e.respondWith(get());
});

