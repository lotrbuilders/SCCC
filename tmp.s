global main:function (main.end-main)
main:
	push ebp
	mov ebp,esp
	pop eax
	push eax
	mov eax,4
	push eax
	pop eax
	push eax
	lea eax,[ebp-4]
	push eax
	pop eax
	push eax
	lea eax,[ebp-8]
	push eax
	pop eax
	push eax
	mov eax,8
	push eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov ebx,eax
	mov eax,[esp]
	mov [ebx],eax
	pop eax
	push eax
	mov eax,[ebp-12]
	mov eax,[eax]
	mov eax,[eax]
	mov esp,ebp
	pop ebp
	ret
.end:
