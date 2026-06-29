import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


ARQUIVO_CSV = "resultados_arvores.csv"
PASTA_SAIDA = Path("graficos")

USAR_ESCALA_LOG = True


def carregar_dados(caminho_csv):
    df = pd.read_csv(caminho_csv, skipinitialspace=True)
    df.columns = df.columns.str.strip()

    colunas_necessarias = [
        "Tamanho",
        "AVL_Ins",
        "AVL_Rem",
        "RN_Ins",
        "RN_Rem",
        "B1_Splits",
        "B1_Fusoes",
        "B1_Emp",
        "B5_Splits",
        "B5_Fusoes",
        "B5_Emp",
        "B10_Splits",
        "B10_Fusoes",
        "B10_Emp",
    ]

    for coluna in colunas_necessarias:
        if coluna not in df.columns:
            raise ValueError(f"Coluna ausente no CSV: {coluna}")

    return df


def preparar_metricas(df):
    df = df.copy()

    # Para a remoção da Árvore B, consideramos os ajustes estruturais:
    # fusões + empréstimos.
    df["B1_Rem"] = df["B1_Fusoes"] + df["B1_Emp"]
    df["B5_Rem"] = df["B5_Fusoes"] + df["B5_Emp"]
    df["B10_Rem"] = df["B10_Fusoes"] + df["B10_Emp"]

    return df


def gerar_grafico(df, titulo, series, nome_arquivo):
    plt.figure(figsize=(11, 6.5))

    for rotulo, coluna in series.items():
        plt.plot(
            df["Tamanho"],
            df[coluna],
            marker="o",
            linewidth=2,
            label=rotulo
        )

    plt.title(titulo)
    plt.xlabel("Tamanho do conjunto de dados")

    if USAR_ESCALA_LOG:
        plt.ylabel("Esforço computacional médio (escala logarítmica)")
        plt.yscale("log")
    else:
        plt.ylabel("Esforço computacional médio")

    plt.grid(True, which="both", linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()

    PASTA_SAIDA.mkdir(exist_ok=True)

    caminho_saida = PASTA_SAIDA / nome_arquivo
    plt.savefig(caminho_saida, dpi=300)
    plt.close()

    print(f"Gráfico salvo em: {caminho_saida}")


def main():
    df = carregar_dados(ARQUIVO_CSV)
    df = preparar_metricas(df)

    series_adicao = {
        "AVL": "AVL_Ins",
        "Rubro-Negra": "RN_Ins",
        "Árvore B - ordem 1": "B1_Splits",
        "Árvore B - ordem 5": "B5_Splits",
        "Árvore B - ordem 10": "B10_Splits",
    }

    series_remocao = {
        "AVL": "AVL_Rem",
        "Rubro-Negra": "RN_Rem",
        "Árvore B - ordem 1": "B1_Rem",
        "Árvore B - ordem 5": "B5_Rem",
        "Árvore B - ordem 10": "B10_Rem",
    }

    gerar_grafico(
        df,
        "Esforço computacional médio na operação de adição",
        series_adicao,
        "grafico_adicao.png"
    )

    gerar_grafico(
        df,
        "Esforço computacional médio na operação de remoção",
        series_remocao,
        "grafico_remocao.png"
    )


if __name__ == "__main__":
    main()