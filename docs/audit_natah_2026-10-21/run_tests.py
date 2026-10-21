#!/usr/bin/env python3
"""Run outside audited source tree: python3 run_tests.py SOURCE OUTPUT."""
import collections, concurrent.futures, hashlib, json, pathlib, re, subprocess, sys, time
root, out = map(lambda x:pathlib.Path(x).resolve(), sys.argv[1:3])
out.mkdir(parents=True, exist_ok=True)
binary = out/'codexion_audit'
def save(name, data):
    (out/name).write_text(json.dumps(data,indent=2))
def run(name, args, prefix=(), timeout=40):
    start=time.monotonic()
    try:
        p=subprocess.run([*prefix,str(binary),*map(str,args)],capture_output=True,text=True,timeout=timeout)
        rc,stdout,stderr=p.returncode,p.stdout,p.stderr
    except subprocess.TimeoutExpired as e:
        rc='timeout';stdout=(e.stdout or b'').decode(errors='replace');stderr=(e.stderr or b'').decode(errors='replace')
    (out/(name+'.log')).write_text(stdout+stderr)
    lines=stdout.splitlines(); events=[]; burns=[];bad=[];pairs=[];short=[];cool=[]
    counts=collections.Counter();phases={};dongles={};last={}
    for i,line in enumerate(lines):
        m=re.fullmatch(r'(\d+) (\d+) (has taken a dongle|is compiling|is debugging|is refactoring|burned out)',line)
        if not m:bad.append(line);continue
        t,c,state=int(m[1]),int(m[2]),m[3]
        if state=='is compiling':
            pairs.append(len(events)>=2 and all(e[1:]==(c,'has taken a dongle') for e in events[-2:]))
            counts[c]+=1;last[c]=t
            for d in (c-1,(c-2)%int(args[0])):
                if d in dongles and t-dongles[d]<int(args[2])+int(args[6]):cool.append([d,dongles[d],t])
                dongles[d]=t
        if state=='burned out':burns.append(dict(line=line,after=len(lines)-i-1,delay=t-last.get(c,0)-int(args[1])))
        if state in ('is compiling','is debugging','is refactoring'):
            if c in phases:
                ot,os=phases[c];minimum=dict(zip(('is compiling','is debugging','is refactoring'),map(int,args[2:5])))[os]
                if t-ot<minimum:short.append([c,os,state,t-ot,minimum])
            phases[c]=(t,state)
        events.append((t,c,state))
    return dict(name=name,args=args,rc=rc,seconds=round(time.monotonic()-start,3),counts=dict(counts),burnouts=burns,
                invalid_lines=bad,pairs_ok=all(pairs),short_phases=short,cooldown_lower_bound_violations=cool,
                tools=[l for l in stderr.splitlines() if any(s in l for s in ('ERROR SUMMARY','in use at exit','definitely lost','All heap blocks'))])
p=subprocess.run(['cc','-Wall','-Wextra','-Werror','-pthread','-g',*map(str,root.glob('*.c')),'-o',str(binary)],capture_output=True,text=True)
(out/'build.log').write_text(p.stdout+p.stderr)
assert p.returncode==0,p.stderr
save('hashes.json',{p.name:hashlib.sha256(p.read_bytes()).hexdigest() for p in root.iterdir() if p.suffix in ('.c','.h')})
cases=[('single',[1,800,200,200,200,10,0,'fifo'],3),('easy_fifo',[5,2000,200,200,200,10,0,'fifo'],3),
('easy_edf',[5,2000,200,200,200,7,0,'edf'],3),('burnout',[5,500,200,200,200,10,0,'fifo'],5),
('cooldown400',[5,3000,200,200,200,10,400,'fifo'],3),('cooldown800_fifo',[5,3000,200,200,200,10,800,'fifo'],3),
('cooldown800_edf',[5,3000,200,200,200,10,800,'edf'],3)]
with concurrent.futures.ThreadPoolExecutor(max_workers=3) as pool:
    res=list(pool.map(lambda x:run(*x),[(f'{n}_{i}',a) for n,a,k in cases for i in range(k)]))
save('benchmarks.json',res)
for r in res:print(json.dumps(r),flush=True)
res=[]
for tool,args,label in [('memcheck',[5,10000,60,60,60,3,60,'edf'],'normal'),('memcheck',[5,500,200,200,200,10,0,'fifo'],'burnout'),
('helgrind',[5,10000,60,60,60,3,60,'edf'],'normal'),('helgrind',[5,500,200,200,200,10,0,'fifo'],'burnout'),('drd',[5,10000,60,60,60,3,60,'edf'],'normal')]:
    opts=['valgrind','--tool='+tool,'--error-exitcode=42']
    if tool=='memcheck':opts+=['--leak-check=full','--show-leak-kinds=all']
    row=run(tool+'_'+label,args,opts);res.append(row);print(json.dumps(row),flush=True)
save('tools.json',res)
res=[run(f'stress_{i}',[200,120,60,60,60,3,60,'edf' if i%2 else 'fifo'],timeout=5) for i in range(100)]
save('stress.json',res)
print('stress after burnout',sum(any(b['after'] for b in r['burnouts']) for r in res),flush=True)
base=['4','2000','60','60','60','2','60','fifo'];cases=[('missing',base[:-1]),('extra',base+['x']),('none',[])]
for pos in range(7):
    for v in ['-1','','-','1x','1.5','+1',' 1','2147483648','999999999999999999999']:
        a=base.copy();a[pos]=v;cases.append((f'arg{pos+1}={v!r}',a))
for v in ['EDF','edf ','','random']:
    a=base.copy();a[-1]=v;cases.append((f'scheduler={v!r}',a))
a=base.copy();a[0]='0';cases.append(('zero_coders',a));res=[]
for name,a in cases:
    try:
        p=subprocess.run([str(binary),*a],capture_output=True,text=True,timeout=3)
        res.append(dict(name=name,args=a,rc=p.returncode,stdout=p.stdout[-1500:],stderr=p.stderr))
    except subprocess.TimeoutExpired:res.append(dict(name=name,args=a,rc='timeout'))
save('parsing.json',res)
print('parsing',len(res),'accepted',sum(r['rc']==0 for r in res),flush=True)
res=[run(f'load_{n}_{s}',[n,10000,60,60,60,3,60,s],timeout=15) for n in [2,3,4,7,31,100,200] for s in ['fifo','edf']]
save('loads.json',res)
print('load failures',[(r['name'],r['rc'],r['burnouts']) for r in res if r['rc']!=0 or r['burnouts']],flush=True)
