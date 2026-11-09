# Arquitetura de Personas Sintéticas e Pesquisa em UX Research

**Documento de arquitetura --- versão final**

## 1. Visão geral

A plataforma tem como objetivo apoiar equipes de UX Research na
exploração de hipóteses e na análise de pesquisas por meio de personas
sintéticas fundamentadas em entrevistas reais.

O pesquisador poderá selecionar pesquisas existentes, gerar uma proposta
de persona com auxílio de IA, validar esse perfil e utilizá-lo em
conversas simuladas. As respostas serão acompanhadas de evidências e
referências às fontes utilizadas, com distinção explícita entre
informações observadas nas entrevistas e inferências produzidas pela IA.

A solução é uma ferramenta de apoio à pesquisa. **Personas sintéticas
não substituem participantes reais nem devem ser apresentadas como
resultados de entrevistas realizadas com pessoas.**

### Objetivos principais

1.  Construir personas a partir de pesquisas reais selecionadas pelo
    pesquisador.
2.  Garantir revisão e aprovação humana antes de disponibilizar uma
    persona para interação.
3.  Permitir conversas contextualizadas por meio de um agente de IA.
4.  Recuperar trechos das pesquisas quando a pergunta exigir evidências
    específicas.
5.  Produzir respostas e relatórios rastreáveis, distinguindo evidências
    de inferências.

## 2. Visão arquitetural

A arquitetura está organizada em três módulos funcionais, conectados por
contratos de dados:

1.  **Criação e gerenciamento de personas:** constrói, organiza,
    versiona e valida os perfis.
2.  **Interação --- Research Chat:** coordena a conversa, utiliza o
    contexto da persona e consulta pesquisas quando necessário.
3.  **Relatórios e evidências:** organiza as respostas, suas
    classificações e as fontes que as fundamentam.

A plataforma também conta com serviços compartilhados de armazenamento,
processamento de documentos, recuperação de informações e autenticação.

``` mermaid
flowchart TB
    U[Pesquisador] --> SSO[SSO corporativo]
    SSO --> UI[Plataforma UX Research]

    UI --> PM[Módulo de gerenciamento de personas]
    UI --> RC[Módulo Research Chat]
    UI --> RP[Módulo de relatórios]

    subgraph KNOW["Serviços compartilhados de pesquisa"]
        UP[Upload de documentos]
        S3[(AWS S3 - documentos originais)]
        L[Processamento - AWS Lambda]
        EX[Extração de texto e chunking]
        EM[Geração de embeddings]
        VS[(Base vetorial)]
        DB[(Banco de dados estruturado)]
        UP --> S3 --> L --> EX --> EM --> VS
    end

    UI --> UP
    PM <--> DB
    RC <--> DB
    RP <--> DB

    PM --> PA[Persona aprovada]
    PA --> AG[Agente de IA]
    RC --> AG
    AG --> CTX[Contexto da persona]
    AG --> DEC{Precisa de trechos das entrevistas?}
    DEC -->|Sim| MCP[MCP Research]
    MCP --> RAG[RAG - recuperação de trechos]
    RAG --> VS
    DEC -->|Não| LLM[LLM / SLM]
    RAG --> LLM
    CTX --> LLM
    LLM --> RESP[Resposta sintética estruturada]
    RESP --> RP
    RP --> OUT[Relatório com evidências e inferências]
```

> O diagrama apresenta a visão lógica. A implementação pode distribuir
> esses componentes em serviços, funções e módulos diferentes, conforme
> as decisões técnicas do projeto.

------------------------------------------------------------------------

## 3. Módulo 1 --- Criação e gerenciamento de personas

**Responsabilidade:** criar, organizar, revisar, aprovar e
disponibilizar personas sintéticas fundamentadas em pesquisas
selecionadas.

### 3.1 Fluxo de criação

``` mermaid
flowchart TD
    A[Gerenciamento de personas] --> B[Selecionar pesquisas]
    B --> C{Existe pesquisa selecionada?}
    C -->|Não| X[Encerrar fluxo e solicitar seleção]
    C -->|Sim| D[Análise com IA]
    D --> E[Persona estruturada]
    E --> F[Validação humana]
    F --> G{Aprovação?}
    G -->|Sim| H[Persona aprovada]
    G -->|Pedir ajuste| E
    H --> I[Disponibilizar para o Research Chat]
```

### 3.2 Gerenciamento de personas

O módulo centraliza as operações relacionadas aos perfis. Deve permitir:

-   Criar uma persona.
-   Consultar personas existentes.
-   Editar informações permitidas.
-   Acompanhar o estado de cada persona.
-   Revisar e aprovar propostas.
-   Consultar versões e histórico.
-   Arquivar personas quando necessário.

O gerenciamento é um módulo funcional; a criação, a edição e a aprovação
são operações realizadas dentro dele, não necessariamente etapas de um
único fluxo linear.

### 3.3 Seleção de pesquisas

O pesquisador seleciona as entrevistas e os materiais que servirão de
base para a construção da persona.

A seleção deve ser registrada e vinculada ao perfil. Se nenhuma pesquisa
for selecionada, a análise não deve ser iniciada.

### 3.4 Análise com IA e persona estruturada

A IA analisa o material selecionado para identificar padrões,
comportamentos, necessidades, motivações, dificuldades e preferências.

A saída é uma proposta estruturada, e não uma persona automaticamente
aprovada.

Exemplo ilustrativo:

  -----------------------------------------------------------------------
  Dimensão                            Característica proposta
  ----------------------------------- -----------------------------------
  Comportamento                       Prefere resolver problemas
                                      bancários de forma independente

  Necessidade                         Sentir segurança durante operações
                                      financeiras

  Dificuldade                         Encontrar determinadas
                                      funcionalidades no aplicativo
  -----------------------------------------------------------------------

Cada característica deve indicar sua sustentação:

-   **Evidência:** vinculada a um trecho identificável de uma
    entrevista.
-   **Inferência:** interpretação produzida pela IA, sem afirmação
    direta correspondente em um trecho específico.

### 3.5 Validação humana

O pesquisador revisa a proposta e pode:

-   **Aprovar:** a persona passa a estar disponível para utilização no
    Research Chat.
-   **Solicitar ajustes:** a proposta retorna para revisão, preservando
    sua identidade e seu histórico.

A persona não pode ser utilizada no Research Chat enquanto não estiver
aprovada.

### 3.6 Estados e armazenamento da persona

As informações da persona devem ser armazenadas em estrutura
persistente, separadas dos documentos originais e dos embeddings.

Dados recomendados:

-   Identificador único da persona.
-   Nome ou título de identificação.
-   Estado: em elaboração, em revisão, aprovada ou arquivada.
-   Características estruturadas.
-   Identificadores das pesquisas e entrevistas de origem.
-   Evidências e inferências associadas a cada característica.
-   Responsável e data da validação.
-   Histórico de alterações e versões.

A aprovação deve registrar qual versão foi validada. Alterações
posteriores relevantes devem gerar uma nova versão ou exigir nova
validação, conforme a política definida pelo produto.

### 3.7 Saída do módulo: persona aprovada

Ao ser aprovada, a persona disponibiliza ao fluxo de interação um
conjunto composto por:

1.  Identificação e versão aprovada da persona.
2.  Características e contexto estruturado.
3.  Referências às entrevistas que deram origem ao perfil.
4.  Evidências associadas às características.
5.  Inferências identificadas como tais.

**O agente recebe mais do que uma descrição de personalidade: recebe um
perfil estruturado, versionado e acompanhado de referências.**

------------------------------------------------------------------------

## 4. Módulo 2 --- Interação: Research Chat

**Responsabilidade:** permitir que o pesquisador converse com uma
persona aprovada, mantendo coerência com seu contexto e consultando as
pesquisas de origem quando necessário.

### 4.1 Fluxo de interação

``` mermaid
flowchart TD
    A[Pesquisador] --> B[Research Chat]
    B --> C[Agente de IA]
    C --> D[Carregar contexto da persona aprovada]
    C --> E{Precisa consultar trechos das entrevistas?}
    E -->|Não| H[LLM / SLM]
    E -->|Sim| F[MCP Research]
    F --> G[RAG - recuperar trechos autorizados]
    G --> H
    D --> H
    H --> I[Resposta sintética estruturada]
```

### 4.2 Research Chat

É a interface em que o pesquisador envia perguntas à persona.

Exemplo:

> Como você se sentiria se o aplicativo solicitasse uma confirmação
> adicional antes de uma transferência?

O chat encaminha a pergunta e a identificação da persona selecionada ao
agente de IA.

### 4.3 Agente de IA e contexto da persona

O agente coordena a interação. Ele deve:

-   Receber a pergunta do pesquisador.
-   Identificar a persona e sua versão aprovada.
-   Carregar o contexto correspondente.
-   Avaliar se a pergunta exige consulta a trechos das entrevistas.
-   Acionar ferramentas de recuperação quando necessário.
-   Encaminhar as informações ao modelo de linguagem.
-   Organizar a resposta e suas referências.

O contexto da persona deve ser considerado em todas as interações. Isso
ajuda a manter consistência entre perguntas e respostas.

### 4.4 Consulta condicional ao MCP Research

Nem toda pergunta exige uma nova busca nas entrevistas.

**Quando o contexto aprovado é suficiente:** o agente pode encaminhar a
pergunta e o contexto diretamente ao modelo.

Exemplo:

> Você prefere processos simples ou complexos?

Se a preferência já estiver documentada no contexto aprovado, uma nova
recuperação de trechos pode ser desnecessária.

**Quando é necessário consultar as entrevistas:** o agente aciona uma
ferramenta do MCP Research para recuperar trechos específicos.

Exemplo:

> Você consegue contar alguma situação em que teve dificuldade para
> realizar uma transferência?

Se a pergunta exige um relato específico que não está detalhado no
contexto consolidado, o agente deve consultar as fontes.

### 4.5 MCP Research e RAG

**MCP --- Model Context Protocol**

O MCP disponibiliza ferramentas que o agente pode utilizar para acessar
recursos e executar consultas.

No projeto, as ferramentas podem permitir:

-   Buscar trechos de entrevistas.
-   Consultar pesquisas associadas a uma persona.
-   Recuperar evidências.
-   Consultar informações estruturadas de personas.

O MCP não é a base vetorial e não substitui o RAG. Ele funciona como uma
interface de ferramentas para o agente acessar os serviços de pesquisa.

**RAG --- Retrieval-Augmented Generation**

O RAG recupera informações relevantes antes da geração da resposta.

O fluxo de recuperação deve:

1.  Receber a consulta do agente.
2.  Restringir a busca às entrevistas vinculadas à versão da persona.
3.  Recuperar os trechos mais relevantes.
4.  Retornar os trechos com identificadores e metadados de origem.
5.  Disponibilizar esse material ao modelo para elaboração da resposta.

A restrição às fontes da persona é essencial para evitar mistura de
pesquisas pertencentes a perfis diferentes.

### 4.6 LLM / SLM

O modelo de linguagem recebe:

-   A pergunta do pesquisador.
-   O contexto da persona aprovada.
-   Os trechos recuperados, quando houver consulta ao RAG.
-   As instruções de comportamento e de classificação da resposta.

O modelo gera uma resposta coerente com o perfil e deve preservar a
distinção entre evidência e inferência.

### 4.7 Formato estruturado da resposta

A resposta não deve ser armazenada apenas como um texto livre.
Recomenda-se uma estrutura que permita rastrear cada afirmação.

Exemplo conceitual:

``` json
{
  "persona_id": "persona-001",
  "persona_version": 3,
  "answer": "Eu me sentiria mais seguro com uma confirmação clara.",
  "claims": [
    {
      "text": "A segurança é importante para este perfil.",
      "classification": "evidence",
      "sources": [
        {
          "interview_id": "interview-014",
          "chunk_id": "chunk-028"
        }
      ]
    },
    {
      "text": "Uma confirmação adicional pode aumentar a tranquilidade.",
      "classification": "inference",
      "sources": []
    }
  ]
}
```

O exemplo é ilustrativo. Os identificadores e campos definitivos devem
ser definidos no contrato de API da solução.

### 4.8 Saída do módulo: resposta sintética

A resposta é uma simulação fundamentada no perfil aprovado e nas
informações recuperadas. Não representa uma declaração real de um
participante.

------------------------------------------------------------------------

## 5. Módulo 3 --- Relatórios e evidências

**Responsabilidade:** organizar as respostas sintéticas, apresentar suas
fontes e permitir que o pesquisador diferencie informações documentadas
de interpretações da IA.

### 5.1 Fluxo do relatório

``` mermaid
flowchart TD
    A[Resposta sintética estruturada] --> B[Classificar afirmações]
    B --> C[Evidências]
    B --> D[Inferências da IA]
    C --> E[Associar trechos e entrevistas de origem]
    D --> F[Identificar como interpretação da IA]
    E --> G[Relatório de pesquisa]
    F --> G
```

### 5.2 Evidências

Uma evidência é uma afirmação sustentada por um trecho identificável de
uma entrevista real.

O sistema deve preservar:

-   O texto da afirmação.
-   O trecho que a sustenta.
-   O identificador da entrevista.
-   O identificador do trecho ou chunk.
-   A relação entre a afirmação e a fonte.

### 5.3 Inferências da IA

Uma inferência é uma interpretação produzida pela IA sem um trecho
específico que sustente diretamente aquela afirmação.

Ela pode ser útil para explorar hipóteses, mas não deve ser apresentada
como fala ou conclusão comprovada de um participante.

As inferências precisam ser identificadas e exibidas separadamente das
evidências.

### 5.4 Conteúdo do relatório

O relatório deve apresentar, no mínimo:

-   Pergunta realizada pelo pesquisador.
-   Persona e versão utilizadas.
-   Resposta sintética.
-   Afirmações classificadas como evidência.
-   Afirmações classificadas como inferência.
-   Trechos e entrevistas de origem, quando aplicável.
-   Identificação clara de que o conteúdo é sintético.

O relatório deve preservar a classificação produzida durante a
interação. Uma inferência não pode ser convertida em evidência durante a
geração ou apresentação do documento.

------------------------------------------------------------------------

## 6. Contratos entre os módulos

Os contratos definem quais informações uma etapa entrega para que outra
consiga continuar o trabalho.

  -----------------------------------------------------------------------
  Origem                  Destino                 Informações
                                                  transferidas
  ----------------------- ----------------------- -----------------------
  Criação                 Interação               Persona aprovada,
                                                  versão,
                                                  características,
                                                  referências e
                                                  entrevistas vinculadas

  Interação               Relatórios              Resposta estruturada,
                                                  afirmações
                                                  classificadas,
                                                  evidências, trechos e
                                                  identificadores de
                                                  origem

  Relatórios              Apresentação            Documento final,
                                                  fontes, classificações
                                                  e identificação de
                                                  conteúdo sintético
  -----------------------------------------------------------------------

### 6.1 Criação → Interação

Somente uma versão aprovada da persona pode ser utilizada. O contrato
deve identificar exatamente qual versão foi validada.

### 6.2 Interação → Relatórios

A interação deve entregar mais do que o texto final. Deve preservar as
afirmações, suas classificações e as referências necessárias para
rastreabilidade.

### 6.3 Relatórios → Apresentação

O relatório apresenta os resultados de forma compreensível, sem misturar
evidências e inferências e sem sugerir que respostas sintéticas sejam
depoimentos reais.

------------------------------------------------------------------------

## 7. Regras de negócio

  -----------------------------------------------------------------------
  Regra                               Comportamento esperado
  ----------------------------------- -----------------------------------
  1\. Somente persona aprovada        O Research Chat não pode utilizar
                                      uma persona em elaboração ou
                                      revisão.

  2\. Seleção vazia                   Sem entrevistas selecionadas, a
                                      análise não é iniciada.

  3\. Solicitação de ajuste           A proposta retorna para revisão,
                                      preservando sua identidade e seu
                                      histórico.

  4\. Contexto persistente            Toda interação considera a versão
                                      aprovada da persona.

  5\. MCP condicional                 A consulta às entrevistas ocorre
                                      quando são necessários trechos
                                      específicos.

  6\. Busca restrita                  O RAG consulta somente as
                                      entrevistas vinculadas à persona
                                      utilizada.

  7\. Evidências rastreáveis          Toda evidência preserva o trecho e
                                      a entrevista de origem.

  8\. Relatório fiel                  O relatório mantém a classificação
                                      de cada afirmação.
  -----------------------------------------------------------------------

### Regras de maior impacto arquitetural

**Persona estruturada não é persona aprovada.** O sistema deve impedir
que uma proposta não validada seja utilizada no chat.

**O MCP é condicional.** A recuperação de trechos não precisa ocorrer em
todas as perguntas; depende da necessidade identificada pelo agente.

**Evidências precisam ser rastreáveis.** Não basta marcar uma afirmação
como evidência: é necessário recuperar o trecho e a entrevista que a
sustentam.

------------------------------------------------------------------------

## 8. Requisitos técnicos e critérios de implementação

Os requisitos abaixo traduzem as regras de negócio em comportamentos
verificáveis.

  -----------------------------------------------------------------------
  ID                      Requisito               Critério de verificação
  ----------------------- ----------------------- -----------------------
  RT-01                   Controlar o estado da   Uma persona não
                          persona                 aprovada não pode
                                                  iniciar uma conversa.

  RT-02                   Preservar a identidade  Solicitar revisão não
                          durante ajustes         cria uma persona
                                                  independente sem
                                                  necessidade.

  RT-03                   Validar a seleção de    A análise não inicia
                          pesquisas               com a seleção vazia.

  RT-04                   Carregar a versão       Toda chamada ao modelo
                          aprovada                recebe o contexto da
                                                  versão aprovada
                                                  selecionada.

  RT-05                   Restringir a            O MCP/RAG consulta
                          recuperação             apenas as entrevistas
                                                  vinculadas à persona.

  RT-06                   Preservar a origem das  Cada evidência mantém
                          evidências              os identificadores do
                                                  trecho e da entrevista.

  RT-07                   Preservar a             O relatório distingue
                          classificação           evidências de
                                                  inferências.

  RT-08                   Identificar conteúdo    Chat e relatório deixam
                          sintético               claro que as respostas
                                                  são simuladas.

  RT-09                   Manter histórico e      Aprovações e alterações
                          versão                  podem ser auditadas.

  RT-10                   Avaliar consistência    É possível testar se
                                                  respostas diferentes
                                                  respeitam o mesmo
                                                  perfil aprovado.
  -----------------------------------------------------------------------

### 8.1 Avaliação das personas

A qualidade de uma persona não deve ser avaliada apenas pela descrição
gerada.

Recomenda-se testar:

-   Consistência do perfil entre perguntas diferentes.
-   Aderência às características aprovadas.
-   Uso correto das entrevistas vinculadas.
-   Separação entre evidência e inferência.
-   Comportamento diante de perguntas sem evidências suficientes.

Esses testes ajudam a identificar respostas contraditórias,
extrapolações e afirmações sem sustentação.

------------------------------------------------------------------------

## 9. Serviços compartilhados e armazenamento

Além dos três módulos funcionais, a arquitetura depende de serviços
compartilhados.

### 9.1 Autenticação e acesso

O pesquisador acessa a plataforma por meio do SSO corporativo. As
permissões devem controlar quem pode consultar pesquisas, criar
personas, aprovar perfis e visualizar relatórios.

### 9.2 Armazenamento de documentos --- AWS S3

O S3 armazena os documentos originais enviados pelos pesquisadores, como
PDFs e transcrições.

O documento original deve permanecer disponível para consulta e
rastreabilidade, respeitando as permissões e políticas de retenção.

### 9.3 Processamento --- AWS Lambda

Funções Lambda podem ser acionadas por eventos de upload para executar o
processamento dos documentos.

Etapas previstas:

1.  Extração do conteúdo textual.
2.  Limpeza e normalização do texto.
3.  Divisão em chunks.
4.  Geração de embeddings.
5.  Persistência dos vetores e metadados na solução de recuperação.

### 9.4 Base vetorial

Armazena os embeddings e os metadados necessários à recuperação
semântica.

A busca deve considerar filtros de escopo, como identificadores das
entrevistas vinculadas à persona, além de controles de acesso.

### 9.5 Banco de dados estruturado

Armazena informações transacionais e relacionais que não devem depender
exclusivamente da base vetorial:

-   Personas e versões.
-   Estados de aprovação.
-   Relação entre personas, pesquisas e entrevistas.
-   Características estruturadas.
-   Conversas e respostas.
-   Afirmações, classificações e referências.
-   Histórico de validações e alterações.
-   Metadados dos relatórios.

**A base vetorial não substitui o armazenamento dos documentos originais
nem o banco de dados estruturado.** Cada componente tem uma
responsabilidade diferente.

------------------------------------------------------------------------

## 10. Exemplo ponta a ponta

Considere uma pesquisadora que deseja entender como usuários de um
aplicativo bancário reagiriam a uma confirmação adicional antes de uma
transferência.

### Etapa 1 --- Seleção e construção

A pesquisadora seleciona três entrevistas. A IA analisa o material e
identifica relatos relacionados à segurança e à clareza das operações.

### Etapa 2 --- Estruturação e validação

A IA propõe uma persona com características organizadas e referências às
entrevistas. A pesquisadora revisa e aprova o perfil.

### Etapa 3 --- Interação

A pesquisadora pergunta:

> Como você se sentiria se o aplicativo solicitasse uma confirmação
> adicional antes de uma transferência?

O agente carrega o contexto aprovado. Se as informações forem
suficientes, pode responder sem nova busca. Se precisar de relatos
específicos, consulta o MCP Research e o RAG, restritos às entrevistas
daquela persona.

### Etapa 4 --- Resposta

A persona sintética responde considerando o perfil aprovado e as
informações recuperadas.

### Etapa 5 --- Relatório

A plataforma organiza a resposta, identifica quais afirmações estão
sustentadas por entrevistas, separa as inferências e apresenta as fontes
correspondentes.

------------------------------------------------------------------------

## 11. Considerações finais

A plataforma propõe um processo controlado para criação e utilização de
personas sintéticas em UX Research.

Seu diferencial não está apenas em permitir conversas com perfis
simulados, mas em manter uma relação verificável entre:

-   As pesquisas reais utilizadas.
-   As características aprovadas da persona.
-   O contexto considerado pelo agente.
-   As respostas sintéticas produzidas.
-   As evidências e inferências apresentadas no relatório.

A validação humana, o controle de versões, a recuperação restrita às
fontes vinculadas e a rastreabilidade das afirmações são elementos
centrais da solução.

**O objetivo é apoiar a exploração de hipóteses e a análise de
pesquisas, oferecendo ao pesquisador uma ferramenta complementar --- sem
substituir a participação de pessoas reais nem confundir simulações com
evidências coletadas diretamente.**
