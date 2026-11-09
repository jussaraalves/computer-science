# Fluxos de Personas e Pesquisa Sintética

## 1. Objetivo da plataforma

A ideia central da plataforma é permitir que um pesquisador construa uma
persona sintética a partir de entrevistas reais, converse com essa
persona e consiga rastrear as respostas até as fontes que fundamentaram
sua construção.

O fluxo completo envolve três processos principais:

1.  Construir uma persona baseada nas entrevistas.
2.  Permitir que o pesquisador converse com essa persona.
3.  Apresentar um relatório que diferencie evidências reais de
    inferências da IA.

Esses processos produzem três artefatos:

  Artefato                Onde é produzido             Quem utiliza
  ----------------------- ---------------------------- ------------------
  Persona aprovada        Criação e validação humana   Agente de IA
  Resposta sintética      Interação                    Chat e relatório
  Relatório de pesquisa   Relatório                    Pesquisador

Um artefato é um resultado produzido em uma etapa e utilizado em outra.
Por exemplo, a persona aprovada é resultado do fluxo de criação e
entrada para o fluxo de interação.

------------------------------------------------------------------------

## 2. Primeiro fluxo: criação da persona

**Fluxo:** Gerenciamento de personas → Seleção de pesquisas → Análise
com IA → Persona estruturada → Validação humana → Persona aprovada.

``` mermaid
flowchart TD
    A[Gerenciamento de personas] --> B[Seleção de pesquisas]
    B --> C[Análise com IA]
    C --> D[Persona estruturada]
    D --> E[Validação humana]
    E --> F[Persona aprovada]
```

### 2.1 Gerenciamento de personas

É o módulo em que o pesquisador pode criar, visualizar e administrar
personas.

A interface pode oferecer opções como:

-   Criar persona.
-   Minhas personas.
-   Personas em elaboração.
-   Personas aprovadas.

Nesse momento, ainda não existe necessariamente uma persona pronta para
conversar. Ela pode estar começando a ser construída.

### 2.2 Seleção de pesquisas

O pesquisador escolhe quais entrevistas serão utilizadas para construir
aquela persona.

Por exemplo, de dez entrevistas disponíveis, ele pode selecionar quatro.
Essas quatro entrevistas serão a base daquele perfil.

**Regra importante:** se a seleção estiver vazia, o fluxo termina antes
da análise. A IA não deve criar um perfil sem material de pesquisa
selecionado para fundamentá-lo.

### 2.3 Análise com IA

A IA analisa as entrevistas selecionadas e procura padrões,
características, comportamentos, necessidades e dificuldades.

Exemplo de padrões encontrados:

-   Três participantes demonstraram preocupação com segurança.
-   Dois tiveram dificuldade para localizar funcionalidades.
-   Quatro preferem resolver problemas sem entrar em contato com o
    atendimento.

A IA deve organizar cada característica com uma referência à entrevista
que a sustenta ou identificá-la como inferência.

### 2.4 Persona estruturada

A análise gera uma proposta organizada para o pesquisador revisar.

Exemplo ilustrativo:

  -----------------------------------------------------------------------
  Característica                      Descrição
  ----------------------------------- -----------------------------------
  Comportamento                       Prefere resolver problemas
                                      bancários de forma independente

  Necessidade                         Ter segurança durante operações
                                      financeiras

  Dificuldade                         Encontrar determinadas
                                      funcionalidades no aplicativo
  -----------------------------------------------------------------------

Cada característica deve estar associada a uma evidência de entrevista
ou identificada como inferência da IA.

É importante distinguir:

-   **Persona estruturada:** proposta produzida pela IA.
-   **Persona aprovada:** perfil que passou pela validação humana.

Não são a mesma coisa.

### 2.5 Validação humana

O pesquisador revisa a proposta e toma uma decisão.

**Caminho A --- Aprovar**

A persona fica disponível para ser utilizada no Research Chat.

**Caminho B --- Pedir ajuste**

A proposta retorna para revisão. O pesquisador corrige as informações
necessárias e envia novamente para validação.

Pedir ajuste não significa criar uma nova persona do zero. O fluxo
retorna à mesma proposta para correção.

### 2.6 O que acontece quando a persona é aprovada?

Quando aprovada, a persona leva consigo:

1.  A lista de entrevistas que deram origem ao perfil.
2.  As características estruturadas.
3.  Para cada característica, o trecho e a entrevista de origem ou a
    identificação de que se trata de uma inferência.

Esse conjunto é entregue ao agente de IA.

**O agente não recebe apenas uma descrição da personalidade. Ele recebe
uma persona acompanhada de suas referências.**

------------------------------------------------------------------------

## 3. Segundo fluxo: interação com a persona

Agora a persona já existe e foi aprovada. O pesquisador pode conversar
com ela pelo Research Chat.

``` mermaid
flowchart TD
    A[Pesquisador] --> B[Research Chat]
    B --> C[Agente de IA]
    C --> D[Contexto da persona]
    C --> E{Precisa consultar trechos das entrevistas?}
    E -->|Não| G[LLM]
    E -->|Sim| F[MCP Research e RAG]
    F --> G
    D --> G
    G --> H[Resposta sintética]
```

### 3.1 Research Chat

É a interface de conversa entre o pesquisador e a persona.

Exemplo de pergunta:

> Como você se sentiria se o aplicativo solicitasse uma confirmação
> adicional antes de uma transferência?

Essa pergunta chega ao agente de IA.

### 3.2 Contexto da persona aprovada

O agente recebe o contexto da persona que passou pela validação.

Exemplo:

-   Valoriza segurança.
-   Prefere processos simples.
-   Tem receio de cometer erros financeiros.
-   Prefere autonomia.

O agente precisa considerar esse perfil durante a interação para manter
a coerência das respostas.

### 3.3 Agente de IA

O agente é responsável pela orquestração do processo.

Ele recebe a pergunta, considera o contexto da persona e decide qual
caminho seguir para produzir a resposta.

Uma decisão importante é verificar se precisa consultar trechos
específicos das entrevistas.

### 3.4 Caminho NÃO: o contexto aprovado é suficiente

Imagine que o pesquisador pergunte:

> Você prefere processos simples ou complexos?

Se o perfil aprovado já contém informações suficientes para responder, o
agente pode encaminhar a pergunta e o contexto diretamente ao modelo.

Nesse caso, não é necessário chamar o MCP Research.

### 3.5 Caminho SIM: é necessário consultar as entrevistas

Agora imagine que o pesquisador pergunte:

> Você consegue contar alguma situação em que teve dificuldade para
> realizar uma transferência?

O perfil consolidado pode indicar que a persona tem dificuldade com
determinadas funcionalidades, mas talvez não contenha o relato original
de uma situação específica.

Nesse caso:

1.  O agente identifica que precisa de um trecho de entrevista.
2.  Aciona uma ferramenta disponibilizada pelo MCP Research.
3.  O MCP permite consultar as pesquisas por meio do RAG.
4.  O RAG recupera trechos das entrevistas selecionadas para aquela
    persona.
5.  O modelo recebe o contexto da persona, a pergunta e os trechos
    recuperados.
6.  O modelo formula a resposta.

**A busca deve ficar restrita às entrevistas que deram origem àquela
persona.** Isso evita que uma persona utilize informações de pesquisas
pertencentes a outro perfil.

### 3.6 MCP Research

MCP significa *Model Context Protocol*.

Ele permite disponibilizar ferramentas para que o agente consulte
informações externas.

No projeto, essas ferramentas podem permitir:

-   Buscar trechos de entrevistas.
-   Consultar pesquisas.
-   Recuperar evidências.
-   Consultar informações de uma persona.

O MCP não é a base vetorial nem substitui o RAG. Ele disponibiliza
ferramentas para o agente acessar os recursos de pesquisa.

### 3.7 LLM/SLM

O documento menciona LLM/SLM:

-   **LLM:** *Large Language Model*.
-   **SLM:** *Small Language Model*.

São modelos de linguagem de diferentes portes.

O modelo recebe as informações necessárias e redige a resposta,
respeitando o contexto aprovado da persona e as evidências recuperadas.

### 3.8 Resposta sintética

É a resposta gerada considerando a persona e as informações disponíveis.

Ela não deve ser tratada como uma declaração de um participante real. É
uma simulação fundamentada nas pesquisas.

------------------------------------------------------------------------

## 4. Terceiro fluxo: relatório e evidências

Essa etapa diferencia a plataforma de um chatbot convencional. A
plataforma não apresenta somente uma resposta: também explica de onde
vieram as informações utilizadas.

Cada afirmação da resposta sintética precisa ser classificada.

Existem duas classificações principais:

### 4.1 Evidência

É uma informação sustentada por um trecho de uma entrevista real.

Exemplo:

A persona diz:

> Eu tenho receio de realizar transferências quando o aplicativo não
> explica claramente cada etapa.

Se existe uma entrevista com um relato correspondente, essa afirmação
pode ser identificada como evidência.

O sistema precisa preservar:

-   O trecho utilizado.
-   A entrevista de origem.
-   A relação entre o trecho e a afirmação.

### 4.2 Inferência da IA

É uma conclusão produzida pela IA, sem um trecho específico que sustente
diretamente aquela afirmação.

Exemplo:

A IA identificou que a preocupação recorrente com segurança pode indicar
que o usuário prefere confirmações adicionais durante operações
financeiras.

Essa interpretação pode ser razoável, mas não significa que algum
participante tenha dito exatamente isso.

Por isso, ela precisa ser identificada como inferência.

### 4.3 Como funciona o relatório?

``` mermaid
flowchart TD
    A[Resposta sintética] --> B[Classificar afirmações]
    B --> C[Evidências]
    B --> D[Inferências da IA]
    C --> E[Identificar trechos e entrevistas de origem]
    D --> F[Identificar como interpretação da IA]
    E --> G[Relatório de pesquisa]
    F --> G
```

O relatório reúne os dois caminhos, mas mantém as classificações
distintas.

Isso permite ao pesquisador consultar uma afirmação e descobrir se ela
veio de uma entrevista ou de uma interpretação da IA.

------------------------------------------------------------------------

## 5. Contratos entre os fluxos

O documento descreve como os três processos se comunicam.

Um contrato é um acordo sobre quais informações uma etapa precisa
entregar para que outra consiga continuar o trabalho.

Existem três conexões principais:

  -----------------------------------------------------------------------
  Origem                  Destino                 O que é transferido
  ----------------------- ----------------------- -----------------------
  Criação                 Interação               Persona aprovada,
                                                  características,
                                                  referências e
                                                  entrevistas

  Interação               Relatório               Resposta classificada,
                                                  evidências, trechos e
                                                  entrevistas

  Relatório               Apresentação            Documento final da
                                                  pesquisa
  -----------------------------------------------------------------------

### 5.1 Criação → Interação

O fluxo de criação entrega a persona aprovada ao fluxo de interação, mas
somente depois da validação humana.

### 5.2 Interação → Relatório

O fluxo de interação entrega a resposta sintética e suas classificações.

Não basta entregar somente uma string com o texto da resposta. É
necessário preservar as informações que explicam a origem de cada
afirmação.

### 5.3 Relatório → Apresentação

O relatório é disponibilizado para o pesquisador apresentar os
resultados da simulação.

As evidências e inferências devem continuar diferenciadas.

------------------------------------------------------------------------

## 6. Regras de negócio

As regras de negócio definem condições que a arquitetura precisa
respeitar.

  -----------------------------------------------------------------------
  Regra                               O que significa na prática
  ----------------------------------- -----------------------------------
  1\. Somente persona aprovada        O chat não pode utilizar uma
                                      persona ainda em elaboração.

  2\. Seleção vazia                   Sem entrevistas selecionadas, a
                                      análise não começa.

  3\. Pedir ajuste                    A revisão retorna à mesma persona
                                      estruturada.

  4\. Contexto persistente            Toda pergunta utiliza o contexto da
                                      persona aprovada.

  5\. MCP condicional                 Só consulta entrevistas quando
                                      precisa de trechos.

  6\. Busca restrita                  O RAG consulta apenas as
                                      entrevistas daquela persona.

  7\. Evidências rastreáveis          Cada evidência preserva o trecho e
                                      a entrevista de origem.

  8\. Relatório fiel                  O relatório mantém a classificação
                                      de cada afirmação.
  -----------------------------------------------------------------------

### Três regras que merecem atenção especial

**Regra 1 --- Persona estruturada não é persona aprovada.**

Se alguém tentar acessar uma persona que ainda está aguardando
validação, o sistema precisa impedir sua utilização no chat.

**Regra 5 --- O MCP é condicional.**

Não precisamos realizar uma busca no RAG em todas as perguntas. Se o
contexto aprovado já for suficiente, o agente pode encaminhar a pergunta
diretamente ao modelo.

**Regra 7 --- Evidência precisa ser rastreável.**

Não basta armazenar que uma afirmação é uma evidência. Precisamos
conseguir recuperar o trecho e a entrevista que a originaram.

------------------------------------------------------------------------

## 7. O que a implementação precisa garantir?

O documento apresenta sete garantias técnicas que ajudam a transformar a
arquitetura em requisitos de desenvolvimento.

1.  **Persona estruturada e persona aprovada são estados diferentes.** O
    agente só pode receber a persona aprovada.

2.  **O pedido de ajuste reabre a mesma persona estruturada.** Não deve
    criar uma nova versão independente sem necessidade.

3.  **Seleção de entrevistas vazia não dispara análise.** É uma
    validação que precisa existir na aplicação.

4.  **Toda chamada ao modelo inclui o contexto da persona aprovada.**
    Isso mantém a coerência entre as respostas.

5.  **O MCP Research é condicional e restrito às entrevistas daquela
    persona.** O agente não deve consultar qualquer pesquisa disponível.

6.  **Evidência persistida carrega trecho e entrevista de origem.**
    Precisamos preservar os vínculos no armazenamento.

7.  **O relatório reproduz a classificação da resposta.** Uma inferência
    não pode aparecer como evidência.

Esses requisitos também ajudam a definir testes automatizados.

Por exemplo, pode existir um teste para verificar se uma persona não
aprovada consegue acessar o Research Chat. O comportamento esperado é
impedir o acesso.

------------------------------------------------------------------------

## 8. Como organizar mentalmente a arquitetura inteira

Em vez de misturar todas as etapas, podemos pensar em três módulos que
se comunicam.

### Módulo 1 --- Criação

**Responsabilidade:** produzir a persona aprovada.

-   Gerenciamento de personas.
-   Seleção de entrevistas.
-   Análise com IA.
-   Persona estruturada.
-   Validação humana.
-   Persona aprovada.

### Módulo 2 --- Interação

**Responsabilidade:** permitir que o pesquisador converse com a persona.

-   Research Chat.
-   Agente de IA.
-   Contexto da persona.
-   MCP Research.
-   RAG.
-   LLM/SLM.
-   Resposta sintética classificada.

### Módulo 3 --- Relatório

**Responsabilidade:** apresentar os resultados com rastreabilidade.

-   Resposta sintética.
-   Evidências.
-   Inferências.
-   Trechos das entrevistas.
-   Fontes de origem.
-   Relatório de pesquisa.

### Visão simplificada

``` mermaid
flowchart TD
    subgraph A["Módulo 1 — Criação"]
        A1[Gerenciamento de personas]
        A2[Seleção de pesquisas]
        A3[Análise com IA]
        A4[Persona estruturada]
        A5[Validação humana]
        A6[Persona aprovada]
        A1 --> A2 --> A3 --> A4 --> A5 --> A6
    end

    subgraph B["Módulo 2 — Interação"]
        B1[Research Chat]
        B2[Agente de IA]
        B3[Contexto da persona]
        B4[MCP Research e RAG]
        B5[LLM/SLM]
        B6[Resposta sintética]
        B1 --> B2
        B2 --> B3
        B2 --> B4
        B3 --> B5
        B4 --> B5
        B5 --> B6
    end

    subgraph C["Módulo 3 — Relatório"]
        C1[Classificação das afirmações]
        C2[Evidências]
        C3[Inferências da IA]
        C4[Relatório de pesquisa]
        C1 --> C2
        C1 --> C3
        C2 --> C4
        C3 --> C4
    end

    A6 --> B2
    B6 --> C1
```

------------------------------------------------------------------------

## 9. Pontos de melhoria na arquitetura

A proposta está bem estruturada, mas existem alguns pontos que podem ser
aprimorados.

### 9.1 Separar criação e gerenciamento

O gerenciamento de personas é um módulo maior.

Dentro dele, podemos ter:

-   Criação.
-   Edição.
-   Aprovação.
-   Consulta.
-   Arquivamento.

Essas funcionalidades não precisam ser representadas como etapas de um
único processo.

### 9.2 Criar um armazenamento estruturado para as personas

Além dos documentos e embeddings, precisamos armazenar:

-   Identificador da persona.
-   Estado (em elaboração, em revisão ou aprovada).
-   Características.
-   Entrevistas relacionadas.
-   Evidências e inferências.
-   Histórico de alterações e aprovações.

### 9.3 Definir um formato estruturado para as respostas

Em vez de o modelo retornar somente texto, ele deveria produzir
informações organizadas por:

-   Afirmação.
-   Classificação.
-   Referências.
-   Trechos de origem, quando aplicável.

Isso facilita a geração do relatório.

### 9.4 Avaliar as personas

Como estamos criando pessoas sintéticas, precisamos avaliar se elas
realmente representam os padrões encontrados nas entrevistas.

Uma persona pode ter uma descrição muito bem construída, mas responder
de maneira inconsistente.

Seria interessante testar a consistência das respostas com perguntas
diferentes e verificar se continuam respeitando o perfil aprovado.

### 9.5 Identificar a simulação

O pesquisador precisa saber que está conversando com uma representação
sintética, não com um participante real.

As respostas não devem ser tratadas como resultados de uma pesquisa
realizada diretamente com pessoas reais.

------------------------------------------------------------------------

## 10. Exercício para fixar os conceitos

### Pergunta 1

Uma persona acabou de ser estruturada pela IA, mas o pesquisador ainda
não aprovou. Ela pode ser utilizada no Research Chat?

-   A)  Sim, porque a IA já terminou a análise.
-   B)  Não, porque somente personas aprovadas podem ser utilizadas.

**Resposta:** B.

A validação humana é o que libera a persona para interação.

### Pergunta 2

O pesquisador pergunta algo que já está explicado no contexto aprovado
da persona. O agente precisa chamar o MCP Research?

-   A)  Não necessariamente. Pode encaminhar o contexto diretamente ao
        modelo.
-   B)  Sim. Toda pergunta precisa consultar as entrevistas.

**Resposta:** A.

A consulta ao MCP é condicional, não obrigatória em todas as perguntas.

### Pergunta 3

A IA apresenta uma conclusão que não aparece diretamente em nenhuma
entrevista. Como ela deve ser classificada?

-   A)  Como evidência.
-   B)  Como inferência da IA.

**Resposta:** B.

Uma conclusão sem trecho de sustentação direta deve ser identificada
como inferência.

------------------------------------------------------------------------

## Conclusão

O que estamos construindo é uma plataforma em que a persona não é
simplesmente um personagem com nome, idade e personalidade.

Ela é um perfil estruturado a partir de pesquisas, validado por um
pesquisador, utilizado por um agente de IA e acompanhado de
rastreabilidade até suas fontes.

A parte mais interessante da ideia é que não basta a IA parecer uma
pessoa: ela precisa conseguir explicar de onde vêm as características
que fazem essa simulação parecer plausível.

**O objetivo é apoiar o trabalho de UX Research, oferecendo uma
ferramenta de exploração e análise, sem substituir a participação de
pessoas reais nas pesquisas.**
