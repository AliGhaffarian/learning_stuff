from bcc import BPF

prog=r"""
BPF_HASH(counter_table);
int hello(void *ctx) {
u64 uid;
u64 counter = 0;
u64 *p;
uid = bpf_get_current_uid_gid() & 0xFFFFFFFF;
p = counter_table.lookup(&uid);
if (p != 0) {
counter = *p;
}
counter++;
counter_table.update(&uid, &counter);
return 0;
}
"""

compiled = BPF(text=prog)
exec_syscall = compiled.get_syscall_fnname("execve")
compiled.attach_kprobe(event=exec_syscall, fn_name="hello")
input("inspect my map using: sudo bpftool map dump id ID")
