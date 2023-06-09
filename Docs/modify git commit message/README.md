# modify git commit message

이 문서는 깃 커밋 메시지 수정에 대한 문서입니다.
> ※ 경고. 커밋 메시지는 신중하게 작성해야 합니다.

<br><br>


## 깃 커밋 메시지 수정하기

깃 커밋 메시지는 항상 신중하게 작성해야 하지만 사람인지라 실수를 할 수도 있습니다. 커밋 메시지를 수정하고 싶은 경우 다음과 같은 경우로 나눌 수 있습니다.

1. 원격 저장소에 push 하지 않은 경우
2. 원격 저장소에 push 한 경우

1번의 경우 로컬에서 수정하면 끝이지만, 2번의 경우는 그렇지 않습니다. 2가지 경우에서 어떻게 메시지를 수정하는지 방법을 살펴보겠습니다.


### 원격 저장소에 push 하지 않은 경우

가장 최근에 메시지를 수정할 경우 다음 명령어를 입력합니다.
```
// 커맨드 라인에서 수정
git commit --amend -m <message>

// 에디터에서 수정
git commit --amend
```

이전의 커밋을 수정할 경우 다음 명령어를 입력합니다.
HEAD를 기준으로 세 번째 커밋을 수정한다고 가정하겠습니다.
```
git rebase -i HEAD~3
```
위 명령어를 사용하면 현재 작업중인 브랜치의 가장 최근 commit 3개를 보여주게 됩니다.
```
pick <hash> <some-message>
pick <hash> <some-message>
pick <hash> <some-message>
```
여기서 수정하고 싶은 커밋(이 경우 3번째) 옆의 `pick`을 `reword`로 바꿔주면 수정할 수 있습니다.


### 원격 저장소에 push 한 경우

위의 원격 저장소에 push 하지 않은 경우처럼 커밋 메시지를 수정한 다음 아래 명령어를 입력합니다.
```
git push --force origin
```

> 이 경우 개인 프로젝트에서는 문제되지 않지만 팀 프로젝트에서는 문제가 발생할 수도 있으므로 권장하지 않는 방식입니다.